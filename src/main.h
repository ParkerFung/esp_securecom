#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <vector>


extern bool newMessageReceived;


struct ReceivedMessage{
  String raw;
  String content;
  String timestamp;
  double latitude = 0.0;
  double longitude = 0.0;
  bool hasCoordinates = false;
};

extern std::vector<ReceivedMessage> receivedMessages;


extern float userLat;
extern float userLng;


extern double targetLat;
extern double targetLng;
extern bool navActive;


void parseMessage(ReceivedMessage& msg);