#ifndef __MQTT_H__
#define __MQTT_H__

#include <ESP8266WiFi.h>
#include "MQTT.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "string.h"


const char* mqtt_server = "19november.freeddns.org";
const int mqtt_port = 5370;

const char* espresso_command_topic_prefix = "/esp/device/";
const char* espresso_command_topic_suffix = "/commands";

const char* healthTopic = "/esp/devices/health";

void setupMQTT(PubSubClient* client) {
  Serial.printf("Using broker %s:%d\n", mqtt_server, mqtt_port);
  client->setServer(mqtt_server, mqtt_port);
}

void publishJsonObject(PubSubClient* client, const char* topic, JsonObject& obj) {
  if (client->connected()) {
    int length = obj.measureLength() + 1;
    char buffer[length];
    obj.printTo(buffer, length);
    client->publish(topic, buffer);
  }
}

void publishString(PubSubClient* client, const char*topic, const char* message) {
  if (client->connected()) {
    client->publish(topic, message);
  }
}

void publishHealthMessage(PubSubClient* client) {
  char buffer[50];
  sprintf(buffer, "{\"serial\":\"%d\",\"free\":%d, \"uptime\": %d}", ESP.getChipId(), ESP.getFreeHeap(), millis());
  publishString(client, healthTopic, buffer);
}

// private method
void reconnect(PubSubClient* client) {
  while (WiFi.status() == WL_CONNECTED && !client->connected()) {
    Serial.print("Attempting MQTT connection...");
    String chipId = String(ESP.getChipId());
    String clientId = "ESP8266Client-" + chipId + "-";
    String commmandTopic = String(espresso_command_topic_prefix) + chipId + String(espresso_command_topic_suffix);
    clientId += String(random(0xffff), HEX);
    if (client->connect(clientId.c_str())) {
      Serial.printf("Subscribe to command topic at %s\n", commmandTopic.c_str());
      client->subscribe(commmandTopic.c_str(), 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
    }
  }
}
// end

void mqttLoop(PubSubClient* client) {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client->connected()) {
      reconnect(client);
    }
    client->loop();
  }
}

#endif
