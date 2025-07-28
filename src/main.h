#include <Wire.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <vector>


extern bool newMessageReceived;


struct ReceivedMessage{
  String content;
  String timestamp;
};

extern std::vector<ReceivedMessage> receivedMessages;
