#ifndef STDARG_C
#define STDARG_C

typedef void *pr_list;

void _pr_start_asm(pr_list *list, void *last_arg);

uint32_t _pr_arg_asm(pr_list list, int i);

#endif //STDARG_C