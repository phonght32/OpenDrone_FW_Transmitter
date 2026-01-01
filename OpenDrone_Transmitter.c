#include "string.h"
#include "OpenDrone_Transmitter.h"
#include "OpenDrone_Transmitter_Config.h"
#include "OpenDrone_Transmitter_HWIF.h"
#include "OpenDrone_TxProtocol.h"
#include "PeriphRadio.h"
#include "PeriphDisplay.h"
#include "PeriphJoystick.h"


#define FREQ_500_HZ_TIME_US         FREQ_HZ_TO_TIME_US(500)
#define FREQ_50_HZ_TIME_US          FREQ_HZ_TO_TIME_US(50)
#define FREQ_30_HZ_TIME_US          FREQ_HZ_TO_TIME_US(30)
#define FREQ_10_HZ_TIME_US          FREQ_HZ_TO_TIME_US(10)
#define FREQ_5_HZ_TIME_US           FREQ_HZ_TO_TIME_US(5)

#define INDEX_SWITCH_ARM_1     		0
#define INDEX_SWITCH_ARM_2      	1

#define TIME_SHOW_INFO_US 	  		2000000
#define TIMEUS_SCREEN_UPDATE 		500000

typedef enum 
{
	OPENDRONE_TRANSMITTER_MAINSTATE_IDLE,
	OPENDRONE_TRANSMITTER_MAINSTATE_SHOW_INFO,
	OPENDRONE_TRANSMITTER_MAINSTATE_WAITING_FOR_ARM,
	OPENDRONE_TRANSMITTER_MAINSTATE_ARMMING,
	OPENDRONE_TRANSMITTER_MAINSTATE_DISARMED,
	OPENDRONE_TRANSMITTER_MAINSTATE_RUNNING,
} OpenDrone_Transmitter_MainState_t;

uint32_t current_time = 0;
uint32_t time_init_done = 0;
uint32_t time_start_arming = 0;
uint32_t time_last_screen_update = 0;
PeriphJoystick_Data_t periph_operator_data = {0};
OpenDrone_TxProtocolMsg_t OpenDrone_TxProtocolMsg = {0};
PeriphSwitch_State_t switch_state[PERIPH_SWITCH_ID_MAX] = {0};
int16_t joystick_data[4] = {0};
OpenDrone_Transmitter_MainState_t main_state = OPENDRONE_TRANSMITTER_MAINSTATE_IDLE;

static void OpenDrone_UpdateData(void);
static void OpenDrone_Transmitter_InitMessage(void);
static void OpenDrone_Transmitter_SendMessageStabilizer(void);
static void OpenDrone_Transmitter_SendMessageArmDisarm(uint8_t arm);


void OpenDrone_Transmitter_Init(void)
{
	OpenDrone_Transmitter_InitMessage();
	PeriphJoystick_Init();
	PeriphRadio_Init();
	PeriphDisplay_Init();
}

void OpenDrone_Transmitter_Main(void)
{
	OpenDrone_UpdateData();

	PeriphRadio_ClearTransmitIrqFlags();
	
	current_time = hwif_get_time_us();

	

	switch (main_state)
	{
	case OPENDRONE_TRANSMITTER_MAINSTATE_IDLE:
		time_init_done = current_time;

		PeriphDisplay_SetState(PERIPH_SCREEN_STATE_SHOW_WELCOME);
		main_state = OPENDRONE_TRANSMITTER_MAINSTATE_SHOW_INFO;
		break;

	case OPENDRONE_TRANSMITTER_MAINSTATE_SHOW_INFO:
		if ((current_time - time_init_done) > TIME_SHOW_INFO_US)
		{
			PeriphDisplay_SetState(PERIPH_SCREEN_STATE_SHOW_WAITING_FOR_ARM);
			main_state = OPENDRONE_TRANSMITTER_MAINSTATE_WAITING_FOR_ARM;
		}
		break;

	case OPENDRONE_TRANSMITTER_MAINSTATE_WAITING_FOR_ARM:
		if ((switch_state[INDEX_SWITCH_ARM_1] == PERIPH_SWITCH_STATE_ON) && (switch_state[INDEX_SWITCH_ARM_2] == PERIPH_SWITCH_STATE_ON))
		{
			PeriphDisplay_SetState(PERIPH_SCREEN_STATE_SHOW_ARMING);
			main_state = OPENDRONE_TRANSMITTER_MAINSTATE_ARMMING;
			time_start_arming = current_time;
		}
		break;
		
	case OPENDRONE_TRANSMITTER_MAINSTATE_ARMMING:
		OpenDrone_Transmitter_SendMessageArmDisarm(1);
		if ((current_time - time_start_arming) > 3000000)
		{
			PeriphDisplay_SetState(PERIPH_SCREEN_STATE_SHOW_RUNNING);
			main_state = OPENDRONE_TRANSMITTER_MAINSTATE_RUNNING;
		}
		break;
	case OPENDRONE_TRANSMITTER_MAINSTATE_RUNNING:
		if ((switch_state[INDEX_SWITCH_ARM_1] == PERIPH_SWITCH_STATE_OFF) && (switch_state[INDEX_SWITCH_ARM_2] == PERIPH_SWITCH_STATE_OFF))
		{
			main_state = OPENDRONE_TRANSMITTER_MAINSTATE_DISARMED;
			OpenDrone_Transmitter_SendMessageArmDisarm(0);
			PeriphDisplay_SetState(PERIPH_SCREEN_STATE_SHOW_WAITING_FOR_ARM);
		}
		else 
		{
			OpenDrone_Transmitter_SendMessageStabilizer();
		}
		break;

	case OPENDRONE_TRANSMITTER_MAINSTATE_DISARMED:
		if ((switch_state[INDEX_SWITCH_ARM_1] == PERIPH_SWITCH_STATE_ON) && (switch_state[INDEX_SWITCH_ARM_2] == PERIPH_SWITCH_STATE_ON))
		{
			PeriphDisplay_SetState(PERIPH_SCREEN_STATE_SHOW_ARMING);
			main_state = OPENDRONE_TRANSMITTER_MAINSTATE_ARMMING;
			time_start_arming = current_time;
		}
		else 
		{
			OpenDrone_Transmitter_SendMessageArmDisarm(0);
		}
		break;
	default:
		break;
	}

	if ((current_time - time_last_screen_update) > TIMEUS_SCREEN_UPDATE)
	{
		PeriphDisplay_Refresh();
		time_last_screen_update = current_time;
	}
}

static void OpenDrone_Transmitter_InitMessage(void)
{
	OpenDrone_TxProtocolMsg.StartInd = 0xAA;
	OpenDrone_TxProtocolMsg.PktLen = 8;
	OpenDrone_TxProtocolMsg.PktSeq = 0x80;
	OpenDrone_TxProtocolMsg.SrcId = 0x40;
	OpenDrone_TxProtocolMsg.DesId = 0x41;
	OpenDrone_TxProtocolMsg.MsgId = OPENDRONE_TXPROTOCOLMSG_ID_STABILIZER_CONTROL;
	OpenDrone_TxProtocolMsg.Crc = 0x00;
	memset(&OpenDrone_TxProtocolMsg.Payload, 0x00, sizeof(OpenDrone_TxProtocolMsg_Payload_t));
}

static void OpenDrone_Transmitter_SendMessageStabilizer(void)
{
	memset(&OpenDrone_TxProtocolMsg, 0x00, sizeof(OpenDrone_TxProtocolMsg_t));

	OpenDrone_TxProtocolMsg.StartInd = 0xAA;
	OpenDrone_TxProtocolMsg.PktLen = 0;
	OpenDrone_TxProtocolMsg.PktSeq = 0x80;
	OpenDrone_TxProtocolMsg.SrcId = 0x40;
	OpenDrone_TxProtocolMsg.DesId = 0x41;
	OpenDrone_TxProtocolMsg.MsgId = OPENDRONE_TXPROTOCOLMSG_ID_STABILIZER_CONTROL;
	OpenDrone_TxProtocolMsg.Crc = 0x00;
	OpenDrone_TxProtocolMsg.Payload.StabilizerCtrl.throttle = periph_operator_data.left_y;
	OpenDrone_TxProtocolMsg.Payload.StabilizerCtrl.roll = periph_operator_data.right_x;
	OpenDrone_TxProtocolMsg.Payload.StabilizerCtrl.pitch = periph_operator_data.right_y;
	OpenDrone_TxProtocolMsg.Payload.StabilizerCtrl.yaw = periph_operator_data.left_x;

	PeriphRadio_Send((uint8_t *)&OpenDrone_TxProtocolMsg);
}

static void OpenDrone_Transmitter_SendMessageArmDisarm(uint8_t arm)
{
	memset(&OpenDrone_TxProtocolMsg, 0x00, sizeof(OpenDrone_TxProtocolMsg_t));

	OpenDrone_TxProtocolMsg.StartInd = 0xAA;
	OpenDrone_TxProtocolMsg.PktLen = 0;
	OpenDrone_TxProtocolMsg.PktSeq = 0x80;
	OpenDrone_TxProtocolMsg.SrcId = 0x40;
	OpenDrone_TxProtocolMsg.DesId = 0x41;
	OpenDrone_TxProtocolMsg.MsgId = OPENDRONE_TXPROTOCOLMSG_ID_ARM_DISARM;
	OpenDrone_TxProtocolMsg.Crc = 0x00;
	OpenDrone_TxProtocolMsg.Payload.ArmDisarm.arm = arm;
	OpenDrone_TxProtocolMsg.Payload.ArmDisarm.confirm = 1;

	PeriphRadio_Send((uint8_t *)&OpenDrone_TxProtocolMsg);
}

static void OpenDrone_UpdateData(void)
{
	for (uint8_t switch_index = 0; switch_index < PERIPH_SWITCH_ID_MAX; switch_index++)
	{
		switch_state[switch_index] = PeiphSwitch_GetState(switch_index);
	}

	PeriphJoystick_GetData(&periph_operator_data);
}
