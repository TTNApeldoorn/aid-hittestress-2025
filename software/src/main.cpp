/*

  Main module

  # Modified by Kyle T. Gabriel to fix issue with incorrect GPS data for TTNMapper

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

#include "utilities.h"
#include "pmu.h"
#include "rom/rtc.h"
#include "AM2315C.h"
#include "ttn.h"
#include "gps.h"
#include "mysps30.h"

// deep sleep support
RTC_DATA_ATTR int bootCount = 0;

// LoRa payload structs
struct Msg {
  int16_t temp, hum, pm2_5, pm10, batt, pm1_0;
};
static Msg msg = {0, 0, 0, 0, 0, 0}; 

struct Stat {
  float lat, lng;
  int16_t alt, hdop, batt, version, cputemp;
};
static Stat stat = {0.0, 0.0, 0, 0, 0, 0, 0}; 

// Sensors
Gps gps;          // GPS sensor
AM2315C am2315;  // temperature hum sensor
Sps30 sps30;     // dust sensor

bool packetSent = false, packetQueued = false;
bool sensorCycle = false, gpsCycle = false;
int start;

// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------

void sleep(int msec) {
  printf("sleep for %d\n", msec);
  ttn_shutdown();
  Serial.flush();
  PMUSavingMode();  // turn off LORA and GPS power
  delay(500);

/*
  // FIXME - use an external 10k pulldown so we can leave the RTC peripherals powered off
  // until then we need the following lines
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

  // Only GPIOs which are have RTC functionality can be used in this bit map: 0,2,4,12-15,25-27,32-39.
  uint64_t gpioMask = (1ULL << BUTTON_PIN);

  // FIXME change polarity so we can wake on ANY_HIGH instead - that would allow us to use all three buttons (instead of just the first)
  gpio_pullup_en((gpio_num_t)BUTTON_PIN);
  esp_sleep_enable_ext1_wakeup(gpioMask, ESP_EXT1_WAKEUP_ALL_LOW);
*/

  esp_sleep_enable_timer_wakeup( msec * 1000ULL);  // call expects usecs
  esp_deep_sleep_start();
}

void _txCallback() {
  // We only want to say 'packetSent' for our packets (not packets needed for joining)
  printf("_txCallback\n");
  if (packetQueued) {
    printf("Message sent\n");
    packetQueued = false;
    packetSent = true;
  }
}

void _rxCallback(unsigned int port, uint8_t* buf, unsigned int len) {
  printf("[TTN] Response: ");
  for (int i = 0; i < len; i++) {
    printf("%02x ", buf[i]);
  }
  printf("\n");
}

void acquireSensorData()
{
  printf("sensor cycle\n");
  // read dust
  if (!sps30.init())  {
    sleep(60000); // Error, do deepsleep retry afer one minute
  }
  if( bootCount == 1 )
    sps30.startManaualFanCleaning();

  if (sps30.read())  {
    msg.pm1_0 = sps30.pm1_0 * 100;
    msg.pm2_5 = sps30.pm2_5 * 100;
    msg.pm10 = sps30.pm10 * 100;
    printf("pm1=%d pm2.5=%d pm10=%d\n", msg.pm1_0, msg.pm2_5, msg.pm10);
  }
  // read temperature and humidity
  if (!am2315.begin())  {
    printf("am2315 module not found!\n");
    sleep(60000); // Error, do deepsleep retry afer one minute
  }
  if (am2315.read() == AM2315C_OK)  {
    msg.temp = am2315.getTemperature() * 100;
    msg.hum = am2315.getHumidity() * 100;
    msg.batt = PMU->getBattVoltage();
    printf("temp=%d  hum=%d\n", msg.temp, msg.hum);
  }
}

void acquireStatusData()
{
  printf("gps cycle\n");
  PMU->enablePowerOutput(XPOWERS_ALDO3);  // set GPS POWER on !!!
  delay(500);  // wait a moment, he power is back on.
  if (!gps.init() )
     sleep(60000); // Error, do deepsleep retry afer one minute
   
  if( gps.read())  {
    stat.lat = gps.lat;
    stat.lng = gps.lng;
    stat.hdop = gps.hdop;
    stat.alt = gps.alt;
  }
  stat.version = APP_VERSION * 100;
  stat.batt = PMU->getBattVoltage();
  stat.cputemp = temperatureRead() * 100;
}

void setup() {
  Serial.begin(115200);

  if( !PMUsetup() ) {
    printf( "PMU error\n");
    sleep( 60000);  // Error, do deep sleep retry afer one minute
  }

  if( bootCount >= 100)   // force a new join after n cycles
    bootCount = 1;
  else
    bootCount++;

  printf( "bootCount = %d\n", bootCount);
  if (bootCount == 1)
    ttn_erase_prefs();

  // TTN setup
  if (!ttn_setup()) {
    printf("Radio module not found!\n");
    sleep( 60000);  //  Error, do deepsleep retry afer one minute
  }

// LoRa Tx Rx callbacks
  ttn_register_rxReady(_rxCallback);
  ttn_register_txReady(_txCallback);

// after n sensor cycles, a statuscycle is processed
  if (bootCount % 10) {      
    sensorCycle = true;
    acquireSensorData();
  }
  // send status and gps position
  else {
    gpsCycle = true;
    acquireStatusData();
  }
  start = millis();
  printf("setup end\r\n");
}

// the loop function processes the send to TTN message 
void loop() {

  ttn_loop();
  
  if( ttn_connected() && !packetQueued && !packetSent) {
    if( sensorCycle )
      ttn_send((uint8_t*)&msg, sizeof(msg), 15);
    else
      ttn_send((uint8_t*)&stat, sizeof(stat), 16);
    packetQueued = true;
  }

  // if packet has been sent, or asume the packet has been sent after 10 sec, 
  // start the deepsleep
  if( packetSent  || millis() - start > 10000) {
    packetSent = false;
    
    if( gpsCycle )
       sleep( 10000);          // do a short sleep, a gps cycle is an in between cycle
    else
      sleep( SEND_INTERVAL);  
  }
}
