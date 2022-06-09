#ifndef APICREGISTER_H
#define APICREGISTER_H

#include <stdint.h>

class APICRegister {
    volatile uint32_t val;
    uint32_t reserved[3];
public:
    volatile uint32_t &operator()() {
        return val;
    }
};

static_assert(sizeof(APICRegister) == sizeof(uint32_t) * 4, "Register must be 4 uint32_t size long");


#endif //APICREGISTER_H
