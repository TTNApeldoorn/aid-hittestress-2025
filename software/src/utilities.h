/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-05-12
 * @last-update 2025-07-07
 */
#pragma once

// Support board list , Macro definition below, select the board definition to be used
// 将要使用的板子型号注释打开,只能打开一个型号,如果不明白自己买的型号是什么,请找客服核对型号


/***********************************  Model definition start 型号定义起始 ***********************************/

//#define T_BEAM_SX1276

/***********************************  Model definition end 型号定义结尾 ***********************************/

#include <Arduino.h>

#define UNUSED_PIN    
#define USING_SX1276               (0)
#define GPS_RX_PIN                  34
#define GPS_TX_PIN                  12
#define BUTTON_PIN                  38
#define BUTTON_PIN_MASK             GPIO_SEL_38
#define I2C_SDA                     21
#define I2C_SCL                     22
#define PMU_IRQ                     35

#define RADIO_SCLK_PIN               5
#define RADIO_MISO_PIN              19
#define RADIO_MOSI_PIN              27
#define RADIO_CS_PIN                18
#define RADIO_DIO0_PIN              26
#define RADIO_RST_PIN               23
#define RADIO_DIO1_PIN              33
// SX1276/78
#define RADIO_DIO2_PIN              32
// SX1262
#define RADIO_BUSY_PIN              32

// LR1121 Only
#define RADIO_DIO9_PIN              33

#define BOARD_LED                   4
#define LED_ON                      LOW
#define LED_OFF                     HIGH

#define BUTTON_PIN                  38

#define GPS_BAUD_RATE               9600
#define HAS_GPS
#define HAS_DISPLAY                 //Optional, bring your own board, no OLED !!
#define HAS_PMU

#define BOARD_VARIANT_NAME          "T-Beam"
#define DISPLAY_MODEL_SSD_LIB       SSD1306Wire
#define RADIO_TYPE_STR  "SX1276"

#define SerialGPS Serial1
#define GPS_MAX_WAIT_FOR_LOCK 120 * 1000
#define SEND_INTERVAL 120 *1000    // the real interval will be ca 25 sec longer, due to PM processing
#define APP_VERSION  4.0

// specify here TTN OTAA keys
#define APPEUI "70B3D57ED001FE1F"
#define APPKEY "46BF7A1A2E959FE9A6E8AE1212E52235"
// DEVEU is obtained from ESP board id



