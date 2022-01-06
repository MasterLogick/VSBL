#ifndef APIC_C
#define APIC_C

#include <stdint.h>
#include <stdbool.h>
#include "Attributes.h"
#include "Register.h"

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
    Register ___reserved0[2];
    Register ID;
    Register version;
    Register ___reserved1[4];
    Register taskPriority;
    Register arbitrationPriority;
    Register processorPriority;
    Register EOI;
    Register remoteRead;
    Register logicalDestination;
    Register destinationFormat;
    Register spuriousInterruptVector;
    Register inService[8];
    Register triggerMode[8];
    Register interruptRequest[8];
    Register errorStatus;
    Register ___reserved2[6];
    Register lvtCorrectedMachineCheckInterrupt;
    Register interruptCommand[2];
    Register lvtTimer;
    Register lvtThermalSensor;
    Register lvtPerformanceMonitoringCounters;
    Register lvtLINT0;
    Register lvtLINT1;
    Register lvtError;
    Register initialCount;
    Register currentCount;
    Register ___reserved3[4];
    Register divideConfiguration;

    uint8_t getID();

    void eoi();

    void initLVT();

    void setTimerDivider(TimerDivider divider);

    void alarmTimer(uint32_t time);

    void enableSpuriousInterrupts();
};

static_assert(sizeof(APIC) == 0x3f0, "APIC must be 0x3f0 bytes long");

APIC *getAPIC();

bool isAPICEnabled();

bool isAPICBSP();

#endif //APIC_C