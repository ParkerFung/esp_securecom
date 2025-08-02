#include <Arduino.h>
#include "main.h"
#include "comm/interface.h"
#include "comm/input.h"
#include "gps/gps.h"
#include "comm/radio.h"
#include "navigation/nav.h"
#include "navigation/nav_interface.h"
#include <vector>

std::vector<ReceivedMessage> receivedMessages;

bool newMessageReceived = false;
bool needsRedraw = true;

Screen currentScreen = MENU;

double targetLat = 0.0;
double targetLng = 0.0;
bool navActive = false;

float userLat = 0.0f;
float userLng = 0.0f;


bool navBlinkState = false;
unsigned long lastBlinkTime = 0;

void setup() {
  Serial.begin(115200);

  initInput();
  initGPS();
  initRadio();
  initDisplay();
}

void loop() {
  readGPS();
  handleInput();

  float currentLat, currentLng;

  String message = receiveMessage();
  if (message.length() > 0) {
    ReceivedMessage newMsg;
    newMsg.raw = message;
    newMsg.timestamp = getTimestamp();

    parseMessage(newMsg);

    receivedMessages.push_back(newMsg);
    newMessageReceived = true;
  }

  if(millis() - lastBlinkTime >= 500){
    navBlinkState = !navBlinkState;
    lastBlinkTime = millis();
  }

  if (currentScreen == NAVIGATION) {
    if (getLocation(currentLat, currentLng)) {
      userLat = currentLat;
      userLng = currentLng;
    }
}

  drawDisplay(currentScreen);
  delay(10);

}

void parseMessage(ReceivedMessage& msg) {
  msg.raw.trim();

  int sepIndex = msg.raw.indexOf('|');
  if (sepIndex != -1) {
    msg.content = msg.raw.substring(0, sepIndex);
    msg.content.trim();

    String coordPart = msg.raw.substring(sepIndex + 1);
    coordPart.trim();

    int commaIndex = coordPart.indexOf(',');
    if (commaIndex != -1) {
      String latStr = coordPart.substring(0, commaIndex);
      String lngStr = coordPart.substring(commaIndex + 1);
      latStr.trim();
      lngStr.trim();

      double lat = latStr.toDouble();
      double lng = lngStr.toDouble();

      if (lat >= -90.0 && lat <= 90.0 && lng >= -180.0 && lng <= 180.0) {
        msg.latitude = lat;
        msg.longitude = lng;
        msg.hasCoordinates = true;
      }
    }
  } else {
    msg.content = msg.raw;
  }
}