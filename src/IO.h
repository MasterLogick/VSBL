#ifndef IO_H
#define IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void _io_write8_port_asm(uint16_t port, uint8_t data);

extern void _io_write16_port_asm(uint16_t port, uint16_t data);

extern void _io_write32_port_asm(uint16_t port, uint32_t data);

extern uint8_t _io_read8_port_asm(uint16_t port);

extern uint16_t _io_read16_port_asm(uint16_t port);

extern uint32_t _io_read32_port_asm(uint16_t port);

#ifdef __cplusplus
}
#endif

#endif //IO_H
