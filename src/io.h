#ifndef IO_H
#define IO_H

#include <stdint.h>

void _io_write8_port_asm(uint16_t port, uint8_t data);

void _io_write16_port_asm(uint16_t port, uint16_t data);

void _io_write32_port_asm(uint16_t port, uint32_t data);

uint8_t _io_read8_port_asm(uint16_t port);

uint16_t _io_read16_port_asm(uint16_t port);

uint32_t _io_read32_port_asm(uint16_t port);

#endif //IO_H
