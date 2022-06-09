#ifndef PORTCONTROLREGISTER_H
#define PORTCONTROLREGISTER_H

#include <stdint.h>
#include "CommandList.h"
#include "ReceivedFrameInformation.h"

struct PortControlRegister {
    CommandList *commandListBaseAddress;
    ReceivedFrameInformation *recievedFIS;
    volatile uint32_t interruptStatus;
    volatile uint32_t interruptEnable;
    volatile uint32_t commandAndStatus;
    volatile uint32_t _RESERVED0;
    volatile uint32_t taskFileData;
    volatile uint32_t signature;
    volatile uint32_t sataStatus;
    volatile uint32_t sataControl;
    volatile uint32_t sataError;
    volatile uint32_t sataActive;
    volatile uint32_t commandIssue;
    volatile uint32_t sataNotification;
    volatile uint32_t fisSwitchingControl;
    volatile uint32_t deviceSleep;
    uint8_t _RESERVED1[0x38];
};
static_assert(sizeof(PortControlRegister) == 0x80, "Port control structure must be packed");


#endif //PORTCONTROLREGISTER_H
