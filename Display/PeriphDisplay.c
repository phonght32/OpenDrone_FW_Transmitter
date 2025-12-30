#include "stdio.h"
#include "PeriphDisplay.h"
#include "OpenDrone_Transmitter_Config.h"
#include "OpenDrone_Transmitter_HWIF.h"
#include "hd44780.h"

#ifdef USE_HD44780_2004
hd44780_handle_t hd44780_handle;
#endif

void PeriphDisplay_Init(void)
{
#ifdef USE_HD44780_2004
	hd44780_handle = hd44780_init();

	hd44780_cfg_t hd44780_cfg = {
		.size = HD44780_SIZE_20_4,
		.comm_mode = HD44780_COMM_MODE_I2C,
		.i2c_send = hwif_hd44780_i2c_send,
		.delay = hwif_delay_ms
	};
	hd44780_set_config(hd44780_handle, hd44780_cfg);
	hd44780_config(hd44780_handle);

	hd44780_clear(hd44780_handle);
	hd44780_gotoxy(hd44780_handle, 0, 6);
	hd44780_write_string(hd44780_handle, (uint8_t *)"Welcome");
	hd44780_gotoxy(hd44780_handle, 1, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)"OpenDroneTransmitter");
#endif
}

void PeriphDisplay_ShowAll(PeriphJoystick_Data_t joystick, PeriphSwitch_State_t *switch_data)
{
	uint8_t row0_data[20];
	uint8_t row1_data[20];

	hd44780_clear(hd44780_handle);

	sprintf((char *)row0_data, "%04i %04i %04i %04i", joystick.left_x, joystick.left_y, joystick.right_x, joystick.right_y);

	hd44780_gotoxy(hd44780_handle, 0, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row0_data);

	sprintf((char *)row1_data, "%01u %01u %01u %01u %01u %01u %01u %01u",
	        switch_data[0], switch_data[1], switch_data[2], switch_data[3],
	        switch_data[4], switch_data[5], switch_data[6], switch_data[7]);

	hd44780_gotoxy(hd44780_handle, 1, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row1_data);
}
