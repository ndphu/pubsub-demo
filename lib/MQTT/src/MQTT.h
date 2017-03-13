#ifndef __MQTT_H__
#define __MQTT_H__
#include <ArduinoJson.h>
#include <PubSubClient.h>

extern const char* mqtt_server;
extern const int mqtt_port;
extern const char* espresso_hello_topic;
extern const char* espresso_command_topic_prefix;
extern const char* espresso_command_topic_suffix;
extern const char* espresso_health_topic_prefix;
extern const char* espresso_health_topic_suffix;
// = "/espresso/hello";

void setupMQTT(PubSubClient*);

void publishJsonObject(PubSubClient*, const char*, const JsonObject&);

void publishString(PubSubClient*,const char*,const char*);

void publishHealthMessage(PubSubClient*);

void reconnect(PubSubClient*);

void mqttLoop(PubSubClient*);

#endif
