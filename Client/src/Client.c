#include "Client.h"
#include "CPUChecker.h"
#include "Protocol.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static int clientSocket = -1;
static struct sockaddr_in serverAddr;
static pthread_t clientThread;
static int isRunning = 0;
static int hasClientThread = 0;

static void tickClient(void)
{
    CPUData cpuData = getCPUData();

    CPULoadProtocol message;
    memset(&message, 0, sizeof(message));

    int cpuDataOk = cpuData.error == CPU_CHECKER_OK;
    message.status = cpuData.error;

    if(cpuDataOk)
    {
        message.totalCPULoad = cpuData.totalLoadPercent;
        message.coreCount = cpuData.coreCount;
        memcpy(message.coreLoads, cpuData.coreLoadPercent, sizeof(message.coreLoads));
    }
    else
        message.totalCPULoad = 0.0;

    ssize_t sent = sendto(clientSocket, &message, sizeof(message), 0,
        (const struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if(sent != sizeof(message))
    {
        perror("Failed to sent data to server");
        return;
    }
}

static void* clientThreadLoop(void* arg)
{
    (void)arg;

    while(isRunning)
    {
        tickClient();
        sleep(1);
    }

    return NULL;
}

int startClient(void)
{
    if(clientSocket >= 0)
        return 1;

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if(clientSocket < 0)
    {
        perror("Failed to create socket");
        return 0;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(!startProcessingCPUFile())
    {
        close(clientSocket);
        clientSocket = -1;
        return 0;
    }

    //Yeah, the whole thread just for this... 
    isRunning = 1;
    if(pthread_create(&clientThread, NULL, clientThreadLoop, NULL) != 0)
    {
        perror("Failed to create client thread");
        isRunning = 0;
        stopProcessingCPUFile();
        close(clientSocket);
        clientSocket = -1;
        return 0;
    }

    hasClientThread = 1;

    return 1;
}

void stopClient(void)
{
    if(hasClientThread)
    {
        isRunning = 0;
        pthread_join(clientThread, NULL);
        hasClientThread = 0;
    }

    if(clientSocket >= 0)
        close(clientSocket);

    clientSocket = -1;

    stopProcessingCPUFile();
}
