#include "ps2.h"
#include "IO.h"
#include "ACPI/ACPI.h"
#include "iostream.h"
#include "util.h"
#include "IDT.h"

#define DATA_PORT 0x60
#define STATUS_PORT 0x64
#define COMMAND_REGISTER 0x64

uint8_t ps2_read_response() {
    while (!(ps2_read_status_register() & 1));
    uint8_t read = _io_read8_port_asm(DATA_PORT);
    return read;
}

void ps2_exec_command(uint8_t command) {
    while (ps2_read_status_register() & 2);
    _io_write8_port_asm(COMMAND_REGISTER, command);
}

bool ps2_init() {
    if (!(GlobalFADT->IAPC_BOOT_ARCH & 0x2)) {
        cout << "PS/2: this PC doesn't have PS/2 controller\n";
        return false;
    }
    _idt_disable_hardware_interrupts_asm();
    ps2_disable_first_port();
    ps2_disable_second_port();
    while (ps2_read_status_register() & 1) {
        ps2_read_response();
    }
    if (!ps2_test_controller()) {
        cout << "PS/2: controller test failed\n";
        return false;
    }
    if (!ps2_test_first_port()) {
        cout << "PS/2: first port test failed\n";
        return false;
    }
    ps2_enable_first_port();
    ps2_write_configuration_byte(0b01100101);
    _idt_enable_hardware_interrupts_asm();
    cout << "PS/2: PS/2 is initialised\n";
    return true;
}

uint8_t ps2_read_status_register() {
    uint8_t status = _io_read8_port_asm(STATUS_PORT);
    return status;
}

uint8_t ps2_read_configuration_byte() {
    ps2_exec_command(0x20);
    return ps2_read_response();
}

void ps2_write_configuration_byte(uint8_t data) {
    ps2_exec_command(0x60);
    _io_write8_port_asm(DATA_PORT, data);
}

bool ps2_test_controller() {
    ps2_exec_command(0xAA);
    return ps2_read_response() == 0x55;
}

bool ps2_test_first_port() {
    ps2_exec_command(0xAB);
    return ps2_read_response() == 0;
}

void ps2_disable_second_port() {
    ps2_exec_command(0xA7);
}

void ps2_enable_second_port() {
    ps2_exec_command(0xA8);
}

void ps2_disable_first_port() {
    ps2_exec_command(0xAD);
}

void ps2_enable_first_port() {
    ps2_exec_command(0xAE);
}

void ps2_write_to_first_device(uint8_t data) {
    ps2_exec_command(data);
}

void ps2_write_to_second_device(uint8_t data) {
    ps2_exec_command(0xD4);
    ps2_exec_command(data);
}

bool ps2_reset_first_device() {
    ps2_write_to_first_device(0xFF);
    return ps2_read_response() == 0xFF;
}

bool ps2_reset_second_device() {
    ps2_write_to_second_device(0xFF);
    return ps2_read_response() == 0xFF;
}