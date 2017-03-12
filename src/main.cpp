#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTT.h"
#include "Secret.h" // define ssid and password
#include "Device.h"
#include "Display.h"


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
  setupMQTT(&client);
  client.setCallback(callback);
  displaySetup();
}

void loop() {
  mqttLoop(&client);
  displayLoop();
}
