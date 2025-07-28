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
  int state = lora.transmit(message);
  
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Send failed, code: ");
    Serial.println(state);
    return false;
  }

  lora.startReceive(); 

  return true;
}

String receiveMessage() {
  String incoming;
  int state = lora.receive(incoming);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.print("Received message: ");
    Serial.println(incoming);
    return incoming;
  } else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("Receive failed, code: ");
    Serial.println(state);
  }

  return "";  // no valid message
}
