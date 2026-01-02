#include "stdio.h"
#include "string.h"
#include "PeriphDisplay.h"
#include "OpenDrone_Transmitter_Config.h"
#include "OpenDrone_Transmitter_HWIF.h"
#include "hd44780.h"

#ifdef USE_HD44780_2004
hd44780_handle_t hd44780_handle;
#endif

PeriphScreen_State_t screen_state = PERIPH_SCREEN_STATE_IDLE;
static PeriphSwitch_State_t info_switch[PERIPH_SWITCH_ID_MAX] = {0};
static PeriphJoystick_Data_t info_joystick = {0};

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
#endif
}

void PeriphDisplay_SetState(PeriphScreen_State_t state)
{
	screen_state = state;
}

void PeriphDisplay_Refresh(void)
{
	switch (screen_state)
	{
	case PERIPH_SCREEN_STATE_SHOW_WELCOME:
		hd44780_clear(hd44780_handle);
		hd44780_gotoxy(hd44780_handle, 0, 6);
		hd44780_write_string(hd44780_handle, (uint8_t *)"Welcome");
		hd44780_gotoxy(hd44780_handle, 1, 0);
		hd44780_write_string(hd44780_handle, (uint8_t *)"OpenDroneTransmitter");
		break;
	case PERIPH_SCREEN_STATE_SHOW_WAITING_FOR_ARM:
		hd44780_clear(hd44780_handle);
		hd44780_gotoxy(hd44780_handle, 0, 1);
		hd44780_write_string(hd44780_handle, (uint8_t *)"Waiting for arm");
		break;
	case PERIPH_SCREEN_STATE_SHOW_ARMING:
		hd44780_clear(hd44780_handle);
		hd44780_gotoxy(hd44780_handle, 0, 1);
		hd44780_write_string(hd44780_handle, (uint8_t *)"Arming ...");
		break;
	case PERIPH_SCREEN_STATE_SHOW_ARM_FAILED:
		hd44780_clear(hd44780_handle);
		hd44780_gotoxy(hd44780_handle, 0, 0);
		hd44780_write_string(hd44780_handle, (uint8_t *)"ARMING FAILED!");
		hd44780_gotoxy(hd44780_handle, 1, 0);
		hd44780_write_string(hd44780_handle, (uint8_t *)"Release throttle");
		break;
	case PERIPH_SCREEN_STATE_SHOW_RUNNING:
		uint8_t row0_data[20];
		uint8_t row1_data[20];

		hd44780_clear(hd44780_handle);

		sprintf((char *)row0_data, "%04i %04i %04i %04i",info_joystick.left_x, info_joystick.left_y, info_joystick.right_x, info_joystick.right_y);

		hd44780_gotoxy(hd44780_handle, 0, 0);
		hd44780_write_string(hd44780_handle, (uint8_t *)row0_data);

		sprintf((char *)row1_data, "%01u %01u %01u %01u %01u %01u %01u %01u",
				info_switch[0], info_switch[1], info_switch[2], info_switch[3],
				info_switch[4], info_switch[5], info_switch[6], info_switch[7]);

		hd44780_gotoxy(hd44780_handle, 1, 0);
		hd44780_write_string(hd44780_handle, (uint8_t *)row1_data);
		break;
	default:
		break;
	}
}

void PeriphDisplay_UpdateData(PeriphSwitch_State_t* switch_state, PeriphJoystick_Data_t* joystick_data)
{
	memcpy(info_switch, switch_state, sizeof(PeriphSwitch_State_t) * PERIPH_SWITCH_ID_MAX);
	memcpy(&info_joystick, joystick_data, sizeof(PeriphJoystick_Data_t));
}


// void PeriphDisplay_ShowAll(PeriphJoystick_Data_t joystick, PeriphSwitch_State_t *switch_data)
// {

// }
