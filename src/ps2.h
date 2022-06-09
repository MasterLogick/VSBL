#ifndef PS2_H
#define PS2_H

#include <stdint.h>

bool ps2_init();

uint8_t ps2_read_response();

uint8_t ps2_read_status_register();

uint8_t ps2_read_configuration_byte();

//todo investigate
//uint8_t ps2_read_byte_from_internal_ram(uint8_t address);

void ps2_write_configuration_byte(uint8_t data);

//todo investigate
//void ps2_write_byte_to_internal_ram(uint8_t address, uint8_t data);

void ps2_disable_second_port();

void ps2_enable_second_port();

uint8_t ps2_test_second_port();

bool ps2_test_controller();

bool ps2_test_first_port();

//todo investigate
//void ps2_diagnostic_dump();

void ps2_disable_first_port();

void ps2_enable_first_port();

//todo investigate
//uint8_t ps2_read_controller_port();

//todo investigate
//void ps2_copy_input_low_to_status();

//todo investigate
//void ps2_copy_input_high_to_status();

uint8_t ps2_read_controller_output_port();

void ps2_write_to_controller_output_port(uint8_t data);

void ps2_write_to_first_port_out_buffer(uint8_t data);

void ps2_write_to_second_port_out_buffer(uint8_t data);

void ps2_write_to_second_port_in_buffer(uint8_t data);

void ps2_pulse_out(bool reset_line);

void ps2_write_to_first_device(uint8_t data);

void ps2_write_to_second_device(uint8_t data);

bool ps2_reset_first_device();

bool ps2_reset_second_device();

#endif //PS2_H
