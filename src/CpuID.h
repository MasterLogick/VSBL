#ifndef CPUID_C
#define CPUID_C

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t _get_cpuid_leaf_asm(uint32_t leaf, uint32_t param, uint32_t register_id);

extern uint32_t CPUID_EAX_ASM;
extern uint32_t CPUID_EBX_ASM;
extern uint32_t CPUID_ECX_ASM;
extern uint32_t CPUID_EDX_ASM;
#define CPUID_EAX (size_t)&CPUID_EAX_ASM
#define CPUID_EBX (size_t)&CPUID_EBX_ASM
#define CPUID_ECX (size_t)&CPUID_ECX_ASM
#define CPUID_EDX (size_t)&CPUID_EDX_ASM

#ifdef __cplusplus
}
#endif

#endif //CPUID_C
