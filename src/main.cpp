#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTT.h"
#include "Secret.h" // define ssid and password 
#include "Device.h"

// WiFiClient

WiFiClient espClient;
PubSubClient client(espClient);

// End WifiClient

// Define Pins
int ledPin = 2;
// End define pins

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  mqttLoop(client);
}
