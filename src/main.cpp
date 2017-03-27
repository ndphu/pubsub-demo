#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "MQTT.h"
#include "Secret.h" // define ssid and password
#include "Device.h"
#include <Ticker.h>

// Wifi client
WiFiClient espClient;
PubSubClient client(espClient);
// End WifiClient

// Health reporter Ticker
const int healthReportInterval = 60; // 1 min for each health report
Ticker healthReporter;
bool healthReportRequested = true;
void reportHealth() {
  //publishString(&client, );
  healthReportRequested = true;
}
// End reporter

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

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  setupMQTT(&client);
  client.setCallback(callback);
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());
  pinMode(ledPin, OUTPUT);
  setup_wifi();
  healthReporter.attach(healthReportInterval, reportHealth);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  mqttLoop(&client);

  if (isHealthReportRequested()) {
    publishHealthMessage(&client);
    setHealthReportRequested(false);
  }

  if (healthReportRequested) {
    publishHealthMessage(&client);
    healthReportRequested = false;
  }
}
