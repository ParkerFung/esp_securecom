#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

extern TinyGPSPlus gps;
extern HardwareSerial GPS;

void initGPS();
void readGPS();
bool getLocation(float& lat, float& lng);
String getTimestamp();

#endif