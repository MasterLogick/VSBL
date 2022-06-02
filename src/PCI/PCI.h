#ifndef PCI_H
#define PCI_H

#include <stdint.h>

class PCI {
private:
public:

    uint32_t readConfigDataRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

    void writeConfigDataRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t data);

    void enumerateFunctions();
};

extern PCI *GlobalPCI;

#endif //PCI_H