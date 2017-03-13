#ifndef __DEVICE_H__
#define __DEVICE_H__

#include  <ESP8266WiFi.h>
#include "Device.h"
#include "Display.h"


const int ledPin = 2;

const char* ACTION_BLINK = "BLINK";
const char* ACTION_GPIO_WRITE = "GPIO_WRITE";
const char* ACTION_DISPLAY_TEXT = "DISPLAY_TEXT";


String getCommandPart(String data, int partIndex) {
  int c = 0;
  int begin = 0;
  int end = 0;
  for (int i = 0; i < data.length(); ++i) {
    if (data[i] == ';') {
      if (partIndex == 0) {
        begin = end;
      } else {
        begin = end + 1;
      }
      end = i;
      if (c++ == partIndex) {
        break;
      }
    }
  }
  return data.substring(begin,end);
}

//
// Command should be in format <number_of_blink>;<delay_time>;
//
void processBlinkAction(String command) {
  int numberOfBlink = atoi(getCommandPart(command, 1).c_str());
  int delayTime = atoi(getCommandPart(command, 2).c_str());
  Serial.printf("Blink %d times with delay %d ms\n", numberOfBlink, delayTime);
  for (int i = 0; i < numberOfBlink; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    delay(delayTime);
  }
}

// GPIO WRITE
void processGpioWrite(String command) {
  int fsc = command.indexOf(";");
  int pin = atoi(getCommandPart(command, 1).c_str());
  int state = atoi(getCommandPart(command, 2).c_str());
  if (state == 0 || state == 1) {
    Serial.printf("Set pin %d to %d\n", pin, state);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state == 0 ? LOW : HIGH);
  }
}

// End GPIO WRITE

// DISPLAY_TEXT
void processDisplayText(String command) {
  setFontSize(atoi(getCommandPart(command, 1).c_str()));
  setTextAlignment(atoi(getCommandPart(command, 2).c_str()));
  setDisplayText(getCommandPart(command, 3));
}
// end DISPLAY_TEXT

void processMessage(String msg) {
  Serial.printf("Process message [%s]\n", msg.c_str());
  int fsc = msg.indexOf(";");
  // String action = msg.substring(0, fsc);
  // String command = msg.substring(fsc + 1);
  String action = getCommandPart(msg, 0);
  if (action  == ACTION_BLINK) {
    processBlinkAction(msg);
  } else if (action  == ACTION_GPIO_WRITE) {
    processGpioWrite(msg);
  } else if (action == ACTION_DISPLAY_TEXT) {
    processDisplayText(msg);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("]");
  // limit the String
  payload[length] = '\0';
  processMessage(String((char*)payload));
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
}

#endif
