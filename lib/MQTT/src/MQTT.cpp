#ifndef __MQTT_H__
#define __MQTT_H__

#include "MQTT.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "string.h"


const char* mqtt_server = "19november.freeddns.org";
const int mqtt_port = 5370;
const char* espresso_hello_topic = "/espresso/device/hello";
const char* espresso_command_topic_prefix = "/espresso/device/";
const char* espresso_command_topic_suffix = "/command";

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

// private method
void reconnect(PubSubClient* client) {
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    String chipId = String(ESP.getChipId());
    String clientId = "ESP8266Client-" + chipId + "-";
    String commmandTopic = String(espresso_command_topic_prefix) + chipId + String(espresso_command_topic_suffix);
    clientId += String(random(0xffff), HEX);
    if (client->connect(clientId.c_str())) {
      publishString(client, espresso_hello_topic, chipId.c_str());
      Serial.printf("Subscribe to command topic at %s\n", commmandTopic.c_str());
      client->subscribe(commmandTopic.c_str(), 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
// end

void mqttLoop(PubSubClient* client) {
  if (!client->connected()) {
    reconnect(client);
  }
  client->loop();
}

#endif
