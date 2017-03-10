#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "Device.h"
#include  <ESP8266WiFi.h>

const int ledPin = 2;

const char* ACTION_BLINK = "BLINK";
//
// Command should be in format <number_of_blink>;<delay_time>;
//
void processBlinkAction(String command) {
  int fsc = command.indexOf(";");
  int numberOfBlink = atoi(command.substring(0, fsc).c_str());
  int delayTime = atoi(command.substring(fsc+1, command.length() - 1).c_str());
  Serial.printf("Blink %d times with delay %d ms\n", numberOfBlink, delayTime);
  for (int i = 0; i < numberOfBlink; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    delay(delayTime);
  }
}

void processMessage(String msg) {
  Serial.printf("Process message [%s]\n", msg.c_str());
  int fsc = msg.indexOf(";");
  String action = msg.substring(0, fsc);
  String command = msg.substring(fsc + 1);
  if (action  == ACTION_BLINK) {
    processBlinkAction(command);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");
  processMessage(String((char*)payload));
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
}

#endif
