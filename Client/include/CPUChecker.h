#ifndef ALT_CPU_CHECKER_H //ALT - Prefix for AnotherLovelyTZ
#define ALT_CPU_CHECKER_H

#include "Protocol.h"

enum CPUCheckerError
{
    CPU_CHECKER_OK = 0,
    FAILED_TO_GET_DATA = 1,
};

typedef struct CPUData
{
    int error;
    double totalLoadPercent;
    int coreCount;
    double coreLoadPercent[ALT_MAX_CPU_CORES];
} CPUData;

typedef struct CPUSnapshot
{
    unsigned long long idle;
    unsigned long long total;
} CPUSnapshot;


int startProcessingCPUFile(void);
CPUData getCPUData(void);
void stopProcessingCPUFile(void);

#endif //ALT_CPU_CHECKER_H
