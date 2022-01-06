#ifndef STDARG_C
#define STDARG_C

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void *pr_list;

void _pr_start_asm(pr_list *list, void *last_arg);

uint32_t _pr_arg_asm(pr_list list, int i);

#ifdef __cplusplus
}
#endif

#endif //STDARG_C