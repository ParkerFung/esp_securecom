#ifndef INTERFACE_H
#define INTERFACE_H

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

extern Adafruit_SSD1306 display;

extern int selectedItemMenu;
extern int selectedItemSend;
extern int selectedItemReceived;
extern int selectedItemAffirmative;
extern int selectedItemNegative;

enum Screen { MENU, SEND, SEND_AFFIRMATIVE, SEND_NEGATIVE, RECEIVED, COORDS, MESSAGE_SENT, NAVIGATION };

extern Screen currentScreen;

void initDisplay();
void drawDisplay(Screen screen);
void drawMenu();
void drawSend();
void drawReceived();
void drawCoords();
void drawSendAffirmative();
void drawSendNegative();
void drawMessageStatus(String msg, String status);






extern const char* affirmativeMessages[];
extern const int affirmativeMessageCount;

extern const char* negativeMessages[];
extern const int negativeMessageCount;

extern const char* distressMessages[];

#endif
