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

#ifndef __PERIPH_DISPLAY_H__
#define __PERIPH_DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "PeriphJoystick.h"
#include "PeriphSwitch.h"

typedef enum
{
    PERIPH_SCREEN_STATE_IDLE = 0,
    PERIPH_SCREEN_STATE_SHOW_WELCOME,
    PERIPH_SCREEN_STATE_SHOW_WAITING_FOR_ARM,
    PERIPH_SCREEN_STATE_SHOW_ARMING,
    PERIPH_SCREEN_STATE_SHOW_ARMING_DONE,
    PERIPH_SCREEN_STATE_SHOW_ARM_FAILED,
    PERIPH_SCREEN_STATE_SHOW_RUNNING
} PeriphScreen_State_t;

void PeriphDisplay_Init(void);
void PeriphDisplay_SetState(PeriphScreen_State_t state);
void PeriphDisplay_Refresh(void);

#ifdef __cplusplus
}
#endif

#endif /* __PERIPH_DISPLAY_H__ */
