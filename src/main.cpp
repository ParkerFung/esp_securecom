#include <Arduino.h>
#include "main.h"
#include "interface.h"
#include "input.h"
#include "gps.h"
#include "radio.h"
#include <vector>


std::vector<ReceivedMessage> receivedMessages;
bool newMessageReceived = false;
bool needsRedraw = true;

Screen currentScreen = MENU;

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

  String message = receiveMessage();
  if (message.length() > 0) {
    ReceivedMessage newMsg;
    newMsg.content = message;
    newMsg.timestamp = getTimestamp();
    receivedMessages.push_back(newMsg);
    newMessageReceived = true;
  }
  drawDisplay(currentScreen);
  delay(10);

}