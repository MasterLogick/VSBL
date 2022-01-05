#ifndef PS2_H
#define PS2_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stdbool.h>

bool ps2_init(void);

uint8_t ps2_read_response(void);

uint8_t ps2_read_status_register(void);

uint8_t ps2_read_configuration_byte(void);

//todo investigate
//uint8_t ps2_read_byte_from_internal_ram(uint8_t address);

void ps2_write_configuration_byte(uint8_t data);

//todo investigate
//void ps2_write_byte_to_internal_ram(uint8_t address, uint8_t data);

void ps2_disable_second_port(void);

void ps2_enable_second_port(void);

uint8_t ps2_test_second_port(void);

bool ps2_test_controller(void);

bool ps2_test_first_port(void);

//todo investigate
//void ps2_diagnostic_dump(void);

void ps2_disable_first_port(void);

void ps2_enable_first_port(void);

//todo investigate
//uint8_t ps2_read_controller_port(void);

//todo investigate
//void ps2_copy_input_low_to_status(void);

//todo investigate
//void ps2_copy_input_high_to_status(void);

uint8_t ps2_read_controller_output_port(void);

void ps2_write_to_controller_output_port(uint8_t data);

void ps2_write_to_first_port_out_buffer(uint8_t data);

void ps2_write_to_second_port_out_buffer(uint8_t data);

void ps2_write_to_second_port_in_buffer(uint8_t data);

void ps2_pulse_out(bool reset_line);

void ps2_write_to_first_device(uint8_t data);

void ps2_write_to_second_device(uint8_t data);

bool ps2_reset_first_device(void);

bool ps2_reset_second_device(void);

#ifdef __cplusplus
}
#endif

#endif //PS2_H
