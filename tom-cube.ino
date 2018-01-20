#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#define TEST 0

#include "Trend.h"
#include "wifipass.h"
#include "parameters.h"
#include "utils.h"

String OTA_HOSTNAME = "ToM-";

String BINARY_SERVER_ADDRESS = "10.10.81.100";
int BINARY_SERVER_PORT = 8000;
String BINARY_SERVER_ENDPOINT = "/bin/" + BINARY_VERSION + "/" + TREND;

String TREND_SERVER_ADDRESS = "10.10.81.100";
int TREND_SERVER_PORT = 8000;
String TREND_SERVER_ENDPOINT = "/panel/cubes/" + TREND;

long SLEEP_MILLIS = 5e3L;
long nextUpdate = 0L;
bool needsReset = false;

Trend mTrend;

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(2, OUTPUT);

  nextUpdate = millis() + SLEEP_MILLIS;
  OTA_HOSTNAME += TREND;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  setupAndStartOTA();
  checkForNewBinary();
}

void update() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin("http://" + TREND_SERVER_ADDRESS + ":" + TREND_SERVER_PORT + TREND_SERVER_ENDPOINT);
  int httpCode = http.GET();
  delay(10);

  if (httpCode == HTTP_CODE_OK) {
    float colorPercent = http.getString().toFloat() / 100.0;
    http.end();
    mTrend.setColor(colorPercent);
  } else {
    http.end();
  }
}

void loop() {
  if (needsReset) reset();

  if (millis() > nextUpdate) {
    update();
    nextUpdate += SLEEP_MILLIS;
  }
  digitalWrite(2, (nextUpdate / SLEEP_MILLIS) % 2);
  ArduinoOTA.handle();
}

