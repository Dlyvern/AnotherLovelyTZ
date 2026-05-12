#ifndef ALT_PROTOCOL_H //ALT - Prefix for AnotherLovelyTZ
#define ALT_PROTOCOL_H

#define ALT_MAX_CPU_CORES 64

typedef struct CPULoadProtocol 
{
    double totalCPULoad;
    int coreCount;
    double coreLoads[ALT_MAX_CPU_CORES];
    int status;
} CPULoadProtocol;

#endif //ALT_PROTOCOL_H
