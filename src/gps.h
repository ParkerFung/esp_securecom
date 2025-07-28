#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>

extern TinyGPSPlus gps;

void initGPS();
void readGPS();
bool getLocation(float& lat, float& lng);
String getTimestamp();

#endif