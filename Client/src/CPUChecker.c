#include "CPUChecker.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static CPUSnapshot previousTotalSnapshot = {0, 0};
static CPUSnapshot previousCoreSnapshots[ALT_MAX_CPU_CORES];
static int hasPreviousTotalSnapshot = 0;
static int hasPreviousCoreSnapshots[ALT_MAX_CPU_CORES];
static int isProcessing = 0;

static int parseCPUSnapshot(char* cpuName, CPUSnapshot* snapshot)
{
    unsigned long long user, nice, system, idle, iowait;
    unsigned long long irq, softirq, steal, guest, guest_nice;

    int parsed = sscanf(
        cpuName,
        "%*s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
        &user,
        &nice,
        &system,
        &idle,
        &iowait,
        &irq,
        &softirq,
        &steal,
        &guest,
        &guest_nice
    );

    if(parsed < 7)
        return 0;

    unsigned long long idleAll = idle + iowait;
    unsigned long long nonIdle = user + nice + system + irq + softirq + steal;

    snapshot->idle = idleAll;
    snapshot->total = idleAll + nonIdle;

    return 1;
}

static double calculateLoadPercent(CPUSnapshot current, CPUSnapshot previous)
{
    unsigned long long totalDelta = current.total - previous.total;
    unsigned long long idleDelta = current.idle - previous.idle;

    if(totalDelta == 0 || totalDelta < idleDelta)
        return 0.0;

    return (double)(totalDelta - idleDelta) * 100.0 / (double)totalDelta;
}

int startProcessingCPUFile(void)
{
    if(isProcessing)
        return 1;

    isProcessing = 1;

    return 1;
}

CPUData getCPUData()
{
    CPUData data;
    memset(&data, 0, sizeof(data));

    if (!isProcessing)
    {
        data.error = FAILED_TO_GET_DATA;
        return data;
    }

    FILE* file = fopen("/proc/stat", "r");

    if (!file)
    {
        perror("Failed to open file");
        data.error = FAILED_TO_GET_DATA;
        return data;
    }

    char line[512];

    if(!fgets(line, sizeof(line), file))
    {
        fclose(file);
        data.error = FAILED_TO_GET_DATA;
        return data;
    }

    CPUSnapshot currentTotalSnapshot;
    if(strncmp(line, "cpu ", 4) != 0 || !parseCPUSnapshot(line, &currentTotalSnapshot))
    {
        fclose(file);
        data.error = FAILED_TO_GET_DATA;
        return data;
    }

    if(hasPreviousTotalSnapshot)
    {
        data.totalLoadPercent = calculateLoadPercent(currentTotalSnapshot, previousTotalSnapshot);
    }

    previousTotalSnapshot = currentTotalSnapshot;
    hasPreviousTotalSnapshot = 1;

    while(fgets(line, sizeof(line), file))
    {
        if(strncmp(line, "cpu", 3) != 0 || !isdigit((unsigned char)line[3]))
            continue;

        if(data.coreCount >= ALT_MAX_CPU_CORES)
            break;

        CPUSnapshot currentCoreSnapshot;
        if(!parseCPUSnapshot(line, &currentCoreSnapshot))
            continue;

        int coreIndex = data.coreCount;
        if(hasPreviousCoreSnapshots[coreIndex])
        {
            data.coreLoadPercent[coreIndex] =
                calculateLoadPercent(currentCoreSnapshot, previousCoreSnapshots[coreIndex]);
        }

        previousCoreSnapshots[coreIndex] = currentCoreSnapshot;
        hasPreviousCoreSnapshots[coreIndex] = 1;
        data.coreCount++;
    }

    fclose(file);

    return data;
}

void stopProcessingCPUFile(void)
{
    if(!isProcessing)
        return;

    isProcessing = 0;
    hasPreviousTotalSnapshot = 0;
    previousTotalSnapshot.idle = 0;
    previousTotalSnapshot.total = 0;
    memset(previousCoreSnapshots, 0, sizeof(previousCoreSnapshots));
    memset(hasPreviousCoreSnapshots, 0, sizeof(hasPreviousCoreSnapshots));
}
