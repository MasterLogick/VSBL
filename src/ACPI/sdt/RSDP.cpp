#include "RSDP.h"
#include "../../string.h"

RSDP *GlobalRSDP;

RSDP *RSDP::find() {
    uint8_t *volatile ptr = reinterpret_cast<uint8_t *>(0x0e0000);
    for (; (uintptr_t) ptr < 0x0fffff; ptr++) {
        if (((RSDP *) ptr)->isValid()) {
            return (RSDP *) ptr;
        }
    }
    return nullptr;
}

bool RSDP::isValid() {
    if (!memcmp(&this->signature, "RSD PTR ", 8))return false;
    uint8_t checksum = 0;
    uint8_t *chr_ptr = (uint8_t *) this;
    for (int i = 0; i < 20; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum)return false;
    if (!revision)return false;
    for (unsigned int i = 20; i < length; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum)return false;
    return true;
}