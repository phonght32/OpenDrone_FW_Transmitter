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

#ifndef __PERIPH_SWITCH_H__
#define __PERIPH_SWITCH_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    PERIPH_SWITCH_ID_1,
    PERIPH_SWITCH_ID_2,
    PERIPH_SWITCH_ID_3,
    PERIPH_SWITCH_ID_4,
    PERIPH_SWITCH_ID_5,
    PERIPH_SWITCH_ID_6,
    PERIPH_SWITCH_ID_7,
    PERIPH_SWITCH_ID_8,
    PERIPH_SWITCH_ID_MAX
} PeriphSwitch_Id_t;

typedef enum 
{
    PERIPH_SWITCH_STATE_OFF = 0,
    PERIPH_SWITCH_STATE_ON,
    PERIPH_SWITCH_STATE_MAX
} PeriphSwitch_State_t;

PeriphSwitch_State_t PeiphSwitch_GetState(PeriphSwitch_Id_t id);

#ifdef __cplusplus
}
#endif

#endif /* __PERIPH_SWITCH_H__ */
