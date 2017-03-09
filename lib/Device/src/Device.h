#ifndef __DEVICE_H__
#define __DEVICE_H__
#include <ESP8266WiFi.h>

void callback(char* topic, byte* payload, unsigned int length);

void processMessage(String msg);

#endif
