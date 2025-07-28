#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>

void initRadio();
bool sendMessage(String& message);
String receiveMessage();

#endif

