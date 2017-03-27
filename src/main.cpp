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
  healthReporter.attach(healthReportInterval, reportHealth);
}

void loop() {
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
