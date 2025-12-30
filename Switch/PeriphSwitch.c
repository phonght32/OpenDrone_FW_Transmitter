#include "PeriphSwitch.h"
#include "OpenDrone_Transmitter_HWIF.h"

PeriphSwitch_State_t PeiphSwitch_GetState(PeriphSwitch_Id_t id)
{
    PeriphSwitch_State_t switch_state = PERIPH_SWITCH_STATE_OFF;

    switch (id) {
    case PERIPH_SWITCH_ID_1:
        switch_state = hwif_switch1_get_state();
        break;
    case PERIPH_SWITCH_ID_2:
        switch_state = hwif_switch2_get_state();
        break;
    case PERIPH_SWITCH_ID_3:
        switch_state = hwif_switch3_get_state();
        break;
    case PERIPH_SWITCH_ID_4:
        switch_state = hwif_switch4_get_state();
        break;
    case PERIPH_SWITCH_ID_5:
        switch_state = hwif_switch5_get_state();
        break;
    case PERIPH_SWITCH_ID_6:
        switch_state = hwif_switch6_get_state();
        break;
    case PERIPH_SWITCH_ID_7:
        switch_state = hwif_switch7_get_state();
        break;
    case PERIPH_SWITCH_ID_8:
        switch_state = hwif_switch8_get_state();
        break;
    default:
        break;
    }

    return switch_state;
}
