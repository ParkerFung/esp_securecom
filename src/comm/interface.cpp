#include "main.h"
#include "interface.h"
#include "gps/gps.h"
#include "navigation/nav.h"
#include "navigation/nav_interface.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int selectedItemMenu = 0;
int selectedItemSend = 0;
int selectedItemReceived = 0;
int selectedItemAffirmative = 0;
int selectedItemNegative = 0;

const char* affirmativeMessages[]{
  "Confirmed",
  "Success",
  "All Clear",
};
const int affirmativeMessageCount = sizeof(affirmativeMessages) / sizeof(affirmativeMessages[0]);

const char* negativeMessages[]{
  "Negative",
  "Failure",
  "Rejected",
};
const int negativeMessageCount = sizeof(negativeMessages) / sizeof(negativeMessages[0]);

const char* distressMessages[]{
  "911"
};

void initDisplay(){
    Wire.begin(21, 4);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED failed to init");
        while (1);
    }
}

void drawDisplay(Screen screen){
  switch(screen){
    case MENU:
      drawMenu();
      break;
    case SEND:
      drawSend();
      break;
    case SEND_AFFIRMATIVE:
      drawSendAffirmative();
      break;
    case SEND_NEGATIVE:
      drawSendNegative();
      break;
    case RECEIVED:
      drawReceived();
      break;
    case COORDS:
      drawCoords();
      break;
    case NAVIGATION:
    if(navActive){
      nav_display(targetLat, targetLng);
    }
    else{

    }
    break;
    default:
      break;
  }
}

void drawMenu(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("SECURE TRANSCIEVER");

  int positions[] = {20, 35, 50};
  const char* menuItems[] = {
    "SEND MESSAGE",
    "RECEIVED MESSAGES",
    "COORDINATES"
  };

  for (int i = 0; i < 3; i++) {
    if (i == selectedItemMenu) {
      display.setCursor(0, positions[i]);
      display.print(">");
    } else {
      display.setCursor(0, positions[i]);
      display.print(" ");
    }
    display.setCursor(10, positions[i]);
    display.println(menuItems[i]);
  }

  if(newMessageReceived){
    display.setCursor(100,20);
    display.print("(!)");
  }

  display.display();
}

void drawSend(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("SEND MESSAGE");

  const char* sendItems[] = {
    "AFFIRMATIVE",
    "NEGATIVE",
    "DISTRESS"
  };
  int positions[] = {20, 35, 50};

  for (int i = 0; i < 3; i++) {
    if (i == selectedItemSend) {
      display.setCursor(0, positions[i]);
      display.print(">");
    } else {
      display.setCursor(0, positions[i]);
      display.print(" ");
    }
    display.setCursor(10, positions[i]);
    display.println(sendItems[i]);
  }

  display.display();
}

void drawSendAffirmative(){

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("AFFIRMATIVE MSG");

  int yPositions[] = {20,35,50,65};
  int count = sizeof(affirmativeMessages) / sizeof(affirmativeMessages[0]);

  for (int i = 0; i < count; i++){
    if (i == selectedItemAffirmative) {
      display.setCursor(0, yPositions[i]);
      display.print(">");
    }
    display.setCursor(10, yPositions[i]);
    display.println(affirmativeMessages[i]);
  }
  display.display();
}

void drawSendNegative(){

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("NEGATIVE MSG");

  int yPositions[] = {20,35,50,65};
  int count = sizeof(negativeMessages) / sizeof(negativeMessages[0]);

  for (int i = 0; i < count; i++){
    if (i == selectedItemNegative) {
      display.setCursor(0, yPositions[i]);
      display.print(">");
    }
    display.setCursor(10, yPositions[i]);
    display.println(negativeMessages[i]);
  }
  display.display();
}

void drawReceived() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("RECEIVED MESSAGES");

  newMessageReceived = false;

  int baseY = 17;
  int lineSpacing = 20; // space for two lines per message

  int itemsToShow = min(2, (int)receivedMessages.size());
  for (int i = 0; i < itemsToShow; i++) {
    int msgIndex = i;
    const ReceivedMessage& msg = receivedMessages[msgIndex];

    int y = baseY + i * lineSpacing;

    display.setCursor(0, y);
    if (msgIndex == selectedItemReceived) {
      display.print(">");
    } else {
      display.print(" ");
    }


    display.setCursor(10, y);
    display.print(msg.content);
    display.print(" | ");
    display.print(msg.timestamp);

    display.setCursor(10, y + 10);
    if (msg.hasCoordinates) {
      display.print(msg.latitude, 6);
      display.print(", ");
      display.print(msg.longitude, 6);
    } else {
      display.print("NO GPS");
    }
  }

  display.display();
}



void drawCoords(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("MY COORDINATES");

  display.setCursor(0,35);

  if(gps.location.isValid()){
    float lat = gps.location.lat();
    float lng = gps.location.lng();

    display.setCursor(0,20);
    display.print("LAT: ");
    display.println(lat, 6);

    display.setCursor(0,35);
    display.print("LNG: ");
    display.println(lng, 6);
  }
  else{
    display.setCursor(0,20);
    display.println("WAITING FOR GPS...");
  }

  display.display();
}

void drawMessageStatus(String msg, String status){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(status);

  display.setCursor(0,20);
  display.println(msg);

  display.display();
}

