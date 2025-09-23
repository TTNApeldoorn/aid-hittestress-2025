
# Homerus Climate sensor 2023
These climate sensors are designed for the Homerus-Energiek project in Apeldoorn.

## Hardware:
- LilyGO TTGO T-Beam V1.2 - LoRa 868MHz - NEO-M8N GPS - ESP32 
- Sensirion SPS30 Particulate matter sensor i2c
- AM2315C Temperature/humidity sensor i2c
- 18650 Li-ion Batterij - 2600mAh
- Hi-Link PCB Supply - 5VDC 1A
- Mini DC-DC 5V Step-up Boost Converter 480mA
- PCB for connecting components
- Kradex housing 176x126x57mm - IP65

Note that there exist several different types of LilyGo T-Beam versions with different power managent modules and LoRa modules.
This documentation describes the **LilyGO T-Beam V1.2** with:
- AXP2101 power module
- SX1276 LoRa module
- NEO-M8N GNSS GPS module

## Software 

### Prerequisites:
- Visual Studio with Platform IO
- mcci-catena/MCCI LoRaWAN LMIC library
- mikalhart/TinyGPSPlus GPS library
- sensirion/sensirion-sps particicle matter SPS30 library
- lewisxhe/AXP202X_Library power control T-Beam
- RobTillaart/AM2315C library for the i2c AM2315C sensor
- espressif32@6.9.0 platform with board lilygo t-beam

The **platformio.ini** takes care for the required settings end libraries. Most libraries are already included in the project sources.

## Instructions
### LoRaWan TTN keys
In the file **configuration.h** the TTN keys APPEUI an APPKEY are defined.
The unique DEVEUI key is obtained from the T-BEAM board id.

## TTN interface
Two type of messages are sent by the sensor:
- measurement report (each 2.5 minutes), send on TTN port 15
- status report (eachtime after 100 measurement reports), send on TTN port 16

Measurement report contains:
- temperature in C
- Relative Humidity in %
- Particulate Matter 1.0 μg/m3
- Particulate Matter 2.5 μg/m3
- Particulate Matter 10 μg/m3
- Battery voltage in V

Status report contains:
- GPS position lat/lon, hdop, altitude
- Sensor version
- batterij voltage

The binary messages from the Sensor are converted by the TTN Payloaddecoder into Json.  
Example JSON measurement message:

> {"pm10":8.5,"pm1p0":7.22,"pm2p5":8.02,"rh":99.99,"temp":7.82,"vbat":4.037}

Example JSON status message:
> {"latitude":52.224578,"longitude":6.0061902,"alt":0.0,"hdop":1.61,vbat":4.099,"SwVer":3.02 }

