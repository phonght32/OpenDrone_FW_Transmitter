#include "stdio.h"
#include "OpenDrone_Transmitter_Config.h"
#include "OpenDrone_Transmitter_HWIF.h"
#include "hd44780.h"

#ifdef USE_HD44780_2004
hd44780_handle_t hd44780_handle;
#endif

err_code_t PeriphDisplay_Init(void)
{
#ifdef USE_HD44780_2004
	hd44780_handle = hd44780_init();

	hd44780_cfg_t hd44780_cfg = {
		.size = HD44780_SIZE_20_4,
		.comm_mode = HD44780_COMM_MODE_I2C,
		.i2c_send = hwif_hd44780_i2c_send,
		.delay = HAL_Delay
	};
	hd44780_set_config(hd44780_handle, hd44780_cfg);
	hd44780_config(hd44780_handle);

	hd44780_clear(hd44780_handle);
	hd44780_gotoxy(hd44780_handle, 0, 6);
	hd44780_write_string(hd44780_handle, (uint8_t *)"Welcome");
	hd44780_gotoxy(hd44780_handle, 1, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)"OpenDroneTransmitter");
#endif
	return ERR_CODE_SUCCESS;
}

err_code_t PeriphDisplay_ShowJoystickData(int16_t left_joystick_x, int16_t left_joystick_y, int16_t right_joystick_x, int16_t right_joystick_y)
{
#ifdef USE_HD44780_2004
	uint8_t row_data[20];

	hd44780_clear(hd44780_handle);

	sprintf((char *)row_data, "X: %i Y: %i", left_joystick_x, left_joystick_y);
	hd44780_gotoxy(hd44780_handle, 0, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row_data);

	sprintf((char *)row_data, "X: %i Y: %0i", right_joystick_x, right_joystick_y);
	hd44780_gotoxy(hd44780_handle, 1, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row_data);
#endif

	return ERR_CODE_SUCCESS;
}

err_code_t PeriphDisplay_ShowStabilizerMessage(int16_t throttle, int16_t roll, int16_t pitch, int16_t yaw)
{
#ifdef USE_HD44780_2004
	uint8_t row0_data[20];
	uint8_t row1_data[20];
	uint8_t row2_data[20];
	uint8_t row3_data[20];

	hd44780_clear(hd44780_handle);

	sprintf((char *)row0_data, "throttle: %05.4i", throttle);
	sprintf((char *)row1_data, "roll    : %05.4i", roll);
	sprintf((char *)row2_data, "pitch   : %05.4i", pitch);
	sprintf((char *)row3_data, "yaw     : %05.4i", yaw);

	hd44780_gotoxy(hd44780_handle, 0, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row0_data);

	hd44780_gotoxy(hd44780_handle, 1, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row1_data);

	hd44780_gotoxy(hd44780_handle, 2, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row2_data);

	hd44780_gotoxy(hd44780_handle, 3, 0);
	hd44780_write_string(hd44780_handle, (uint8_t *)row3_data);
#endif

	return ERR_CODE_SUCCESS;
}
