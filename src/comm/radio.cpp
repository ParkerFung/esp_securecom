#include "radio.h"
#include <RadioLib.h>

// LoRa Pins
#define LORA_CS     10
#define LORA_DIO1   7
#define LORA_RST    9
#define LORA_BUSY   8
#define LORA_RXEN   2
#define LORA_TXEN   5

SX1262 lora = new Module(LORA_CS, LORA_DIO1, LORA_RST, LORA_BUSY);
static int messageID = 0;
int lastAckId = -1;

void initRadio(){
  int state = lora.begin();


  if (state != RADIOLIB_ERR_NONE){
    Serial.print("lora failed to init. error code: ");
    Serial.println(state);
    while(true);
  }


  lora.setFrequency(915.0);
  lora.setSpreadingFactor(9);
  lora.setBandwidth(125.0);
  lora.setCodingRate(7); 
  lora.setOutputPower(17);
  lora.setCRC(true);
  lora.setRfSwitchPins(LORA_RXEN, LORA_TXEN);


  lora.startReceive();

  Serial.println("LoRa initialized");

}

bool sendMessage(String& message) {
  static int messageIdCounter = 0;
  int msgId = messageIdCounter++;
  String fullMessage = "MSG|" + String(msgId) + "|" + message;

  const int maxRetries = 3;
  const unsigned long ackTimeout = 1000;

  for (int attempt = 0; attempt < maxRetries; attempt++) {
    Serial.print("Sending (attempt ");
    Serial.print(attempt + 1);
    Serial.println("): " + fullMessage);

    int state = lora.transmit(fullMessage);
    delay(10);
    lora.startReceive();

    if (state != RADIOLIB_ERR_NONE) {
      Serial.println("Transmission failed.");
      continue;
    }

    unsigned long startTime = millis();
    while (millis() - startTime < ackTimeout) {
      String ack = checkForAck();
      if (ack.startsWith("ACK|")) {
        int ackId = ack.substring(4).toInt();
        if (ackId == msgId) {
          Serial.println("ACK received!");
          return true;
        }
      }
      delay(50);
    }

    Serial.println("ACK not received, retrying...");
  }

  Serial.println("Failed to get ACK after retries.");
  return false;
}

String receiveMessage() {
  String incoming;
  int state = lora.receive(incoming);
  lora.startReceive();

  if (state == RADIOLIB_ERR_NONE) {
    incoming.trim();
    Serial.print("Received message: ");
    Serial.println(incoming);

    if (incoming.startsWith("MSG|")) {
      int firstBar = incoming.indexOf('|');
      int secondBar = incoming.indexOf('|', firstBar + 1);
      if (firstBar != -1 && secondBar != -1) {
        String msgIdStr = incoming.substring(firstBar + 1, secondBar);
        msgIdStr.trim();
        String ack = "ACK|" + msgIdStr;
        lora.transmit(ack);
        lora.startReceive();

        return incoming.substring(secondBar + 1);  // actual message
      }
    }

    return incoming;  // fallback (may include malformed messages)
  } else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("Receive failed, code: ");
    Serial.println(state);
  }

  return "";
}
String checkForAck() {
  String incoming;
  int state = lora.receive(incoming);
  lora.startReceive();

  if (state == RADIOLIB_ERR_NONE) {
    incoming.trim();
    if (incoming.startsWith("ACK|")) {
      return incoming;  // return the full ACK string
    }
  }
  return "";  // no ACK
}
