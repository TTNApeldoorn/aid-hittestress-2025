/**
 * @file      boards.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  ShenZhen XinYuan Electronic Technology Co., Ltd
 * @date      2024-04-25
 * @last-update 2024-08-07
 */

#pragma once

#include <SPI.h>
#include <Wire.h>
#include <XPowersLib.h>

bool PMUsetup();
void PMUSavingMode();

extern XPowersLibInterface *PMU;
extern bool pmuInterrupt;

