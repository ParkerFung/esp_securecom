#include "main.h"
#include "input.h"
#include "interface.h"
#include "radio.h"
#include "gps.h"

extern int selectedItemMenu;
extern int selectedItemSend;
extern int selectedItemReceived;
extern int selectedItemAffirmative;
extern int selectedItemNegative;

#define LONG_PRESS_DURATION 500 // milliseconds

void initInput(){
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
}

void handleInput() {

  static unsigned long selectPressStart = 0;
  static bool selectPressed = false;
  static bool longPressTriggered = false;

  int* selectedItemPtr = nullptr;
  int maxItems = 0;

  switch (currentScreen) {
    case MENU:
      selectedItemPtr = &selectedItemMenu;
      maxItems = 3; // number of menu items
      break;
    case SEND:
      selectedItemPtr = &selectedItemSend;
      maxItems = 3; // send submenu items count
      break;
    case SEND_AFFIRMATIVE:
      selectedItemPtr = &selectedItemAffirmative;
      maxItems = affirmativeMessageCount;
      break;
    case SEND_NEGATIVE:
      selectedItemPtr = &selectedItemNegative;
      maxItems = negativeMessageCount;
      break;
    case RECEIVED:
      selectedItemPtr = &selectedItemReceived;
      maxItems = 3; // received messages count 
      break;
    case COORDS:
      // No selectable items in coordinates screen
      break;
  }

  if (selectedItemPtr != nullptr) {
    if (digitalRead(BUTTON_DOWN) == LOW) {
      (*selectedItemPtr)++;
      if (*selectedItemPtr >= maxItems) {
        *selectedItemPtr = 0;
      }
      delay(200);
    }

    if (digitalRead(BUTTON_UP) == LOW) {
      if (*selectedItemPtr == 0) {
        *selectedItemPtr = maxItems - 1;
      } else {
        (*selectedItemPtr)--;
      }
      delay(50);
    }
  }

  if (digitalRead(BUTTON_SELECT) == LOW) {
    if (!selectPressed) {
      selectPressStart = millis();
      selectPressed = true;
      longPressTriggered = false;
    } else {
      if (!longPressTriggered && (millis() - selectPressStart >= LONG_PRESS_DURATION)) {
        currentScreen = MENU;
        selectedItemMenu = 0;
        selectedItemSend = 0;
        selectedItemReceived = 0;
        longPressTriggered = true;
        delay(50);
      }
    }
  } else if (selectPressed) {
    if (!longPressTriggered) {
      if (currentScreen == MENU) {
        switch (selectedItemMenu) {
          case 0:
            currentScreen = SEND;
            break;
          case 1:
            currentScreen = RECEIVED;
            break;
          case 2:
            currentScreen = COORDS;
            break;
        }
        delay(50);
      }
      else if (currentScreen == SEND){
        switch(selectedItemSend){
          case 0: //affirmative messages
            currentScreen = SEND_AFFIRMATIVE;
            break;
          case 1: //negative messages
            currentScreen = SEND_NEGATIVE;
            break;
          case 2: //distress
            //distress message logic
            break;
        }
      }
      else if (currentScreen == SEND_AFFIRMATIVE){
        // send affirmative message
        float lat, lng;
        String msg = String(affirmativeMessages[selectedItemAffirmative]) + " | ";
        if (getLocation(lat, lng)){
          msg += String(lat, 6) + ", " + String(lng, 6);
        }
        else{
          msg += "NO GPS";
        }
        sendMessage(msg);
        drawMessageSent(msg);
        delay(5000);

        currentScreen = MENU;
        selectedItemMenu = 0;
        selectedItemSend = 0;
        selectedItemAffirmative = 0;

        delay(100);
      }
      else if (currentScreen == SEND_NEGATIVE){
        // send negative message
        float lat, lng;
        String msg = String(negativeMessages[selectedItemNegative]) + " | ";
        if(getLocation(lat,lng)){
          msg += String(lat, 6) + "," + String(lng, 6);
        }
        else{
          msg += "NO GPS";
        }
        sendMessage(msg);
        drawMessageSent(msg);


        currentScreen = MENU;
        selectedItemMenu = 0;
        selectedItemSend = 0;
        selectedItemNegative = 0;

      }
    }
    selectPressed = false;
  }
}
