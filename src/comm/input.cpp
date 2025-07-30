#include "main.h"
#include "input.h"
#include "interface.h"
#include "radio.h"
#include "gps/gps.h"

extern int selectedItemMenu;
extern int selectedItemSend;
extern int selectedItemReceived;
extern int selectedItemAffirmative;
extern int selectedItemNegative;

extern bool navActive;
extern double targetLat;
extern double targetLng;

static int receivedScrollOffset = 0;

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
      maxItems = receivedMessages.size(); // received messages count 
      if (maxItems == 0) maxItems = 1;
      break;
    case COORDS:
    case NAVIGATION:
      // No selectable items in coordinates or navigation screen
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


  // Handle select button press/release logic
  if (digitalRead(BUTTON_SELECT) == LOW) {
    if (!selectPressed) {
      selectPressStart = millis();
      selectPressed = true;
      longPressTriggered = false;
    } else {
      if (!longPressTriggered && (millis() - selectPressStart >= LONG_PRESS_DURATION)) {
        // Long press anywhere returns to MENU and stops navigation
        currentScreen = MENU;
        selectedItemMenu = 0;
        selectedItemSend = 0;
        selectedItemReceived = 0;
        navActive = false;
        longPressTriggered = true;
        delay(50);
      }
    }
  } else if (selectPressed) {
    // Select button released - short press actions
    if (!longPressTriggered) {
      if (currentScreen == NAVIGATION) {
        // Short press in NAVIGATION stops nav and goes back to MENU
        navActive = false;
        currentScreen = MENU;
        selectedItemMenu = 0;
      }
      else if (currentScreen == MENU) {
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
            // distress message logic
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
        drawMessageStatus(msg, "Sending...");
        bool success = sendMessage(msg);

        if(success){
          drawMessageStatus(msg, "Message Sent");
        }
        else{
          drawMessageStatus(msg, "Send Failed");
        }


        delay(3000);

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
        drawMessageStatus(msg, "Sending...");
        bool success = sendMessage(msg);

        if(success){
          drawMessageStatus(msg, "Message Sent");
        }
        else{
          drawMessageStatus(msg, "Send Failed");
        }


        delay(3000);

        currentScreen = MENU;
        selectedItemMenu = 0;
        selectedItemSend = 0;
        selectedItemNegative = 0;
      }
      else if (currentScreen == RECEIVED){
        if (selectedItemReceived < receivedMessages.size()){
          const ReceivedMessage& msg = receivedMessages[selectedItemReceived];

          if(msg.hasCoordinates){
            targetLat = msg.latitude;
            targetLng = msg.longitude;
            navActive = true;
            currentScreen = NAVIGATION;
          }
          else{
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 20);
            display.println("NO GPS AVAILABLE");
            display.println("FOR NAVIGATION");
            display.display();
            delay(2000);
          }
        }
        delay(100);
      }
    }
    selectPressed = false;
  }
}

