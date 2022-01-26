#include "DescriptionHeader.h"

bool DescriptionHeader::isValid() {
    uint8_t checksum = 0;
    for (uint8_t *i = (uint8_t *) this; (uint32_t) (i - (uint8_t *) this) + 1 <= length; ++i) {
        checksum += *i;
    }
    return !checksum;
}