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

#ifndef __OPENDRONE_TRANSMITTER_DEFINE_H__
#define __OPENDRONE_TRANSMITTER_DEFINE_H__

#include "OpenDrone_TxProtocol.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USE_HD44780_2004

#define USE_JOYSTICK_MODULE

#define USE_NRF24L01
// #define USE_SX1278


#define SCALE_RATIO_ADC 				(128/4096)


#define CONFIG_NRF24L01_CHANNEL				2500
#define CONFIG_NRF24L01_PAYLOAD_LEN 		OPENDRONE_TXPROTOCOL_MSG_LEN
#define CONFIG_NRF24L01_CRC_LEN  			1
#define CONFIG_NRF24L01_ADDR_WIDTH  		5
#define CONFIG_NRF24L01_RETRANS_CNT  		3
#define CONFIG_NRF24L01_RETRANS_DELAY 		200
#define CONFIG_NRF24L01_DATA_RATE  			NRF24L01_DATA_RATE_1Mbps
#define CONFIG_NRF24L01_OUTPUT_PWR 			NRF24L01_OUTPUT_PWR_0dBm
#define CONFIG_NRF24L01_TRANSCEIVER_MODE  	NRF24L01_TRANSCEIVER_MODE_TX


#define CONFIG_SX1278_FREQ 					434000000
#define CONFIG_SX1278_PACKET_LEN  			OPENDRONE_TXPROTOCOL_MSG_LEN
#define CONFIG_SX1278_OUTPUT_PWR  			SX1278_OUTPUT_PWR_17dBm
#define CONFIG_SX1278_SPREAD_FACTOR  		SX1278_SPREAD_FACTOR_7
#define CONFIG_SX1278_BANDWIDTH  			SX1278_BANDWIDTH_125kHz
#define CONFIG_SX1278_CODING_RATE  			SX1278_CODING_RATE_4_5
#define CONFIG_SX1278_CRC_EN  				SX1278_CRC_ENABLE
#define CONFIG_SX1278_TRANSCEIVER_MODE 		SX1278_TRANSCEIVER_MODE_TX

#ifdef __cplusplus
}
#endif

#endif /* __OPENDRONE_TRANSMITTER_DEFINE_H__ */
