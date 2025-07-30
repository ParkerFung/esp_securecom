#include "gps.h"


HardwareSerial GPS(1);
TinyGPSPlus gps;

//gps pins
#define GPS_RX_PIN 16  // GPS TX -> ESP32 RX
#define GPS_TX_PIN 3   // GPS RX <- ESP32 TX



void initGPS(){

  // Setup GPS UART
  GPS.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);


}

void readGPS(){
  while (GPS.available()){
    gps.encode(GPS.read());
  }
}

bool getLocation(float& lat, float& lng) {
  if (gps.location.isValid()) {
    lat = gps.location.lat();
    lng = gps.location.lng();
    return true;
  }
  return false;
}

String getTimestamp() {
  if (gps.time.isValid()) {
    int utcHour = gps.time.hour();
    int localHour = utcHour - 5;

    if (localHour < 0) {
      localHour += 24;
    }

    char buf[6]; 
    snprintf(buf, sizeof(buf), "%02d:%02d", localHour, gps.time.minute());
    return String(buf);
  }
  return String("??:??");
}