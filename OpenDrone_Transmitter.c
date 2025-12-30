#include "string.h"
#include "OpenDrone_Transmitter.h"
#include "OpenDrone_Transmitter_Config.h"
#include "OpenDrone_Transmitter_HWIF.h"
#include "OpenDrone_TxProtocol.h"
#include "PeriphRadio.h"
#include "PeriphDisplay.h"
#include "PeriphJoystick.h"

#define FREQ_HZ_TO_TIME_US(x)       (1000000.0f/(x))
#define TIME_US_TO_FREQ_HZ(x)       (1000000.0f/(x))

#define IDX_TASK_500_HZ             0
#define IDX_TASK_50_HZ              1
#define IDX_TASK_30_HZ              2
#define IDX_TASK_10_HZ              3
#define IDX_TASK_5_HZ               4
#define NUM_OF_TASK                 5

#define FREQ_500_HZ_TIME_US         FREQ_HZ_TO_TIME_US(500)
#define FREQ_50_HZ_TIME_US          FREQ_HZ_TO_TIME_US(50)
#define FREQ_30_HZ_TIME_US          FREQ_HZ_TO_TIME_US(30)
#define FREQ_10_HZ_TIME_US          FREQ_HZ_TO_TIME_US(10)
#define FREQ_5_HZ_TIME_US           FREQ_HZ_TO_TIME_US(5)

#define INDEX_SWITCH_ARM_DISARM     0
#define INDEX_SWITCH_START      	1

uint32_t last_time_us[NUM_OF_TASK] = {0};
PeriphJoystick_Data_t periph_operator_data = {0};
OpenDrone_TxProtocolMsg_t OpenDrone_TxProtocolMsg = {0};
PeriphSwitch_State_t switch_state[PERIPH_SWITCH_ID_MAX] = {0};
int16_t joystick_data[4] = {0};
uint8_t is_armed = 0;

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
	for (uint8_t switch_index = 0; switch_index < PERIPH_SWITCH_ID_MAX; switch_index++)
	{
		switch_state[switch_index] = PeiphSwitch_GetState(switch_index);
	}

	if (is_armed == 0 && switch_state[INDEX_SWITCH_ARM_DISARM] == PERIPH_SWITCH_STATE_ON)
	{
		OpenDrone_Transmitter_SendMessageArmDisarm(1);
		is_armed = 1;
	}
	else if (is_armed == 1 && switch_state[INDEX_SWITCH_ARM_DISARM] == PERIPH_SWITCH_STATE_OFF)
	{
		OpenDrone_Transmitter_SendMessageArmDisarm(0);
		is_armed = 0;
	}

	uint32_t current_time = hwif_get_time_us();

	/* Task 500 Hz */
	if ((current_time - last_time_us[IDX_TASK_500_HZ]) > FREQ_500_HZ_TIME_US)
	{
		PeriphRadio_ClearTransmitIrqFlags();

		last_time_us[IDX_TASK_500_HZ] = current_time;
	}

	/* Task 50 Hz */
	if ((current_time - last_time_us[IDX_TASK_50_HZ]) > FREQ_50_HZ_TIME_US)
	{
		PeriphJoystick_GetData(&periph_operator_data);

		if (switch_state[INDEX_SWITCH_START] == PERIPH_SWITCH_STATE_ON)
		{
			OpenDrone_Transmitter_SendMessageStabilizer();
		}

		last_time_us[IDX_TASK_50_HZ] = current_time;
	}

	/* Task 30 Hz */
	if ((current_time - last_time_us[IDX_TASK_30_HZ]) > FREQ_30_HZ_TIME_US)
	{
		last_time_us[IDX_TASK_30_HZ] = current_time;
	}

	/* Task 5 Hz */
	if ((current_time - last_time_us[IDX_TASK_5_HZ]) > FREQ_5_HZ_TIME_US)
	{
		PeriphDisplay_ShowAll(periph_operator_data, switch_state);

		last_time_us[IDX_TASK_5_HZ] = current_time;
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
