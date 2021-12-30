#ifndef APIC_C
#define APIC_C

#include <stdint.h>
#include <stdbool.h>

typedef char *apic_base;

enum timer_divider {
    DIV_2 = 0b0000,
    DIV_4 = 0b0001,
    DIV_8 = 0b0010,
    DIV_16 = 0b0011,
    DIV_32 = 0b1000,
    DIV_64 = 0b1001,
    DIV_128 = 0b1010,
    DIV_1 = 0b1011
};
typedef enum timer_divider timer_divider;

apic_base apic_get_base();

bool apic_is_enabled(void);

bool apic_is_bsp(void);

uint32_t apic_get_version_register(apic_base base);

void apic_init_lvt(apic_base base);

void apic_set_timer_divider(apic_base base, timer_divider divider);

void apic_alarm_timer(apic_base base, uint32_t time);

uint32_t apic_get_current_count_register(apic_base base);

void apic_enable_spurious_interrupts(apic_base base);

uint32_t apic_get_errors(apic_base base);

#endif //APIC_C