// MIT License

// Copyright (c) 2024 phonght32

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __OPENDRONE_TRANSMITTER_HWIF_H__
#define __OPENDRONE_TRANSMITTER_HWIF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf24l01.h"
#include "sx1278.h"
#include "joystick.h"
#include "hd44780.h"
#include "PeriphSwitch.h"
#include "OpenDrone_Transmitter_Config.h"

uint32_t hwif_get_time_us(void);
void hwif_delay_ms(uint32_t time_ms);

#ifdef USE_HD44780_2004
hd44780_status_t hwif_hd44780_i2c_send(uint8_t *buf_send, uint16_t len);
#endif

#ifdef USE_JOYSTICK_MODULE
joystick_status_t hwif_left_joystick_get_pos_x(uint16_t *pos_x);
joystick_status_t hwif_left_joystick_get_pos_y(uint16_t *pos_y);
joystick_status_t hwif_left_joystick_get_bt_status(uint8_t *bt_status);
joystick_status_t hwif_right_joystick_get_pos_x(uint16_t *pos_x);
joystick_status_t hwif_right_joystick_get_pos_y(uint16_t *pos_y);
joystick_status_t hwif_right_joystick_get_bt_status(uint8_t *bt_status);
#endif

#ifdef USE_NRF24L01
nrf24l01_status_t hwif_nrf24l01_spi_send(uint8_t *buf_send, uint16_t len);
nrf24l01_status_t hwif_nrf24l01_spi_recv(uint8_t *buf_recv, uint16_t len);
nrf24l01_status_t hwif_nrf24l01_set_cs(uint8_t level);
nrf24l01_status_t hwif_nrf24l01_set_ce(uint8_t level);
nrf24l01_status_t hwif_nrf24l01_get_irq(uint8_t *level);
#endif

#ifdef USE_SX1278
sx1278_status_t hwif_sx1278_spi_send(uint8_t *buf_send, uint16_t len);
sx1278_status_t hwif_sx1278_spi_recv(uint8_t *buf_recv, uint16_t len);
sx1278_status_t hwif_sx1278_set_cs(uint8_t level);
sx1278_status_t hwif_sx1278_set_rst(uint8_t level);
sx1278_status_t hwif_sx1278_get_irq(uint8_t *level);
#endif

PeriphSwitch_State_t hwif_switch1_get_state(void);
PeriphSwitch_State_t hwif_switch2_get_state(void);
PeriphSwitch_State_t hwif_switch3_get_state(void);
PeriphSwitch_State_t hwif_switch4_get_state(void);
PeriphSwitch_State_t hwif_switch5_get_state(void);
PeriphSwitch_State_t hwif_switch6_get_state(void);
PeriphSwitch_State_t hwif_switch7_get_state(void);
PeriphSwitch_State_t hwif_switch8_get_state(void);

#ifdef __cplusplus
}
#endif

#endif /* __OPENDRONE_TRANSMITTER_HWIF_H__ */
