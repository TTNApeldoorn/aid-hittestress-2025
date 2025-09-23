/*

  GPS module

  Copyright (C) 2018 by Xose Pérez <xose dot perez at gmail dot com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <TinyGPS++.h>
#include "utilities.h"
#include "gps.h"

static TinyGPSPlus tinyGpsPlus;

boolean Gps::init() {
  lat = lng = 0.0;
  hdop = alt = 0;
  SerialGPS.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(100);
  int start = millis();
  while( millis() - start < 5000) {
    if( SerialGPS.available() > 10)
      return true;
  }
  printf("No GPS detected: check wiring and/or power\n");
  return false;
}

bool Gps::read()
{
  printf("Gps::read\n");

  bool fix = false;
  int i = 0;
  long start = millis();

  // leave loop after valid location or after timout
  do
  {
    while (SerialGPS.available() > 0)
    {
      tinyGpsPlus.encode(SerialGPS.read());
      i++;
    }
    fix = tinyGpsPlus.location.isValid();
  } while (!fix && millis() - start < GPS_MAX_WAIT_FOR_LOCK);

  lat = tinyGpsPlus.location.lat();
  lng = tinyGpsPlus.location.lng();
  hdop = 1000 * tinyGpsPlus.hdop.hdop();
  alt = tinyGpsPlus.altitude.meters();

  printf("GPS chars read:%d\n", i);
  printf("Location: %f, %f\n", lat, lng);

  return fix;
}
