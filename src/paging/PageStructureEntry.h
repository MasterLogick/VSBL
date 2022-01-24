#ifndef PAGESTRUCTUREENTRY_H
#define PAGESTRUCTUREENTRY_H

#include "../Attributes.h"
#include <stdint.h>

#define PageStrcutureEntry(name, body...) \
struct name {                          \
    uint64_t                           \
    present: 1,                        \
    rw: 1,                             \
    user: 1,                           \
    pwt: 1,                            \
    pcd: 1,                            \
    accessed: 1,                       \
    dirty: 1,                          \
    body,                              \
    :7,                                \
    protectionKey: 4,                  \
    execDisable: 1;                    \
} PACKED;                              \
static_assert(sizeof(name) == 8);

#endif //PAGESTRUCTUREENTRY_H
