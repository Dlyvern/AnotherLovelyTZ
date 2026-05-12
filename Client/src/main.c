#include "Client.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t shouldStop = 0;

static void handleSigint(int /*signal*/)
{
    shouldStop = 1;
}

int main(void)
{
    signal(SIGINT, handleSigint);

    if(!startClient())
    {
        printf("Failed to start client\n");
        return 1;
    }

    while (!shouldStop)
    {
        sleep(1);
    }

    stopClient();

    printf("Stopping client...\n");

    return 0;
}
