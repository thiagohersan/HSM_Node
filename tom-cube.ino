#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#include "parameters.h"
#include "tom-cube.h"
#include "Trend.h"

Trend mTrend;

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetup");
  pinMode(2, OUTPUT);

  randomSeed(analogRead(A0));
  nextTrendUpdate = millis() + (TREND_UPDATE_PERIOD_MILLIS + 1e3L * random(0, 2));
  nextBinaryUpdate = millis() + (BINARY_UPDATE_PERIOD_MILLIS + 1e3L * random(-10, 10));

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

  for (int counter = 0; (WiFi.status() != WL_CONNECTED) && (counter < 32); counter++) {
    delay(500);
  }

  TREND = getTrendFromMacAddress(WiFi.macAddress());
  OTA_HOSTNAME += TREND;
  TREND_SERVER_ENDPOINT += TREND;

  setupAndStartOTA();
}

void updateTrend() {
  HTTPClient http;
  http.begin("http://" + TREND_SERVER_ADDRESS + ":" + TREND_SERVER_PORT + TREND_SERVER_ENDPOINT);
  int httpCode = http.GET();
  delay(10);

  if (httpCode == HTTP_CODE_OK) {
    float colorPercent = http.getString().toFloat() / 100.0;
    http.end();
    mTrend.sweepColor(colorPercent);
  } else {
    http.end();
  }
}

void loop() {
  if (needsReset) reset();

  if (millis() > nextTrendUpdate) {
    if (WiFi.status() == WL_CONNECTED) updateTrend();
    nextTrendUpdate += (TREND_UPDATE_PERIOD_MILLIS + 1e3L * random(0, 2));
  }

  if (millis() > nextBinaryUpdate) {
    if (WiFi.status() == WL_CONNECTED) checkForNewBinary();
    nextBinaryUpdate += (BINARY_UPDATE_PERIOD_MILLIS + 1e3L * random(-10, 10));
  }

  digitalWrite(2, (nextTrendUpdate / TREND_UPDATE_PERIOD_MILLIS) % 2);
  ArduinoOTA.handle();
}

