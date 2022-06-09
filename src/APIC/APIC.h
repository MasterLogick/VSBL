#ifndef APIC_C
#define APIC_C

#include <stdint.h>
#include <stdbool.h>
#include <Attributes.h>
#include "APICRegister.h"

enum TimerDivider {
    DIV_2 = 0b0000,
    DIV_4 = 0b0001,
    DIV_8 = 0b0010,
    DIV_16 = 0b0011,
    DIV_32 = 0b1000,
    DIV_64 = 0b1001,
    DIV_128 = 0b1010,
    DIV_1 = 0b1011
};

struct APIC {
    APICRegister ___reserved0[2];
    APICRegister ID;
    APICRegister version;
    APICRegister ___reserved1[4];
    APICRegister taskPriority;
    APICRegister arbitrationPriority;
    APICRegister processorPriority;
    APICRegister EOI;
    APICRegister remoteRead;
    APICRegister logicalDestination;
    APICRegister destinationFormat;
    APICRegister spuriousInterruptVector;
    APICRegister inService[8];
    APICRegister triggerMode[8];
    APICRegister interruptRequest[8];
    APICRegister errorStatus;
    APICRegister ___reserved2[6];
    APICRegister lvtCorrectedMachineCheckInterrupt;
    APICRegister interruptCommand[2];
    APICRegister lvtTimer;
    APICRegister lvtThermalSensor;
    APICRegister lvtPerformanceMonitoringCounters;
    APICRegister lvtLINT0;
    APICRegister lvtLINT1;
    APICRegister lvtError;
    APICRegister initialCount;
    APICRegister currentCount;
    APICRegister ___reserved3[4];
    APICRegister divideConfiguration;

    uint8_t getID();

    void eoi();

    void initLVT();

    void setTimerDivider(TimerDivider divider);

    void alarmTimer(uint32_t time);

    void enableSpuriousInterrupts();

    static APIC *getAPIC();
};

static_assert(sizeof(APIC) == 0x3f0, "APIC must be 0x3f0 bytes long");

bool isAPICEnabled();

bool isAPICBSP();

#endif //APIC_C