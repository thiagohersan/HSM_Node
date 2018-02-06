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

int TREND_ORDER[] = {7, 12, 18, 8, 4, 21, 24, 22, 16, 2, 17, 11, 9, 10, 23, 5, 3, 1, 15, 20, 13, 6, 14, 19};
int TREND_ORDER_SIZE = sizeof(TREND_ORDER) / sizeof(TREND_ORDER[0]);

Trend mTrend;

float currentPercent;
float maxPercent;

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(2, OUTPUT);

  randomSeed(analogRead(A0));
  nextTrendUpdate = millis() + (TREND_UPDATE_PERIOD_MILLIS + 1e3L * random(0, 2));
  nextBinaryUpdate = millis() + (BINARY_UPDATE_PERIOD_MILLIS + 1e3L * random(-10, 10));

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  TREND = WiFi.localIP()[3];
  OTA_HOSTNAME += TREND;
  TREND_SERVER_ENDPOINT += TREND;

  currentPercent = 0;
  maxPercent = (float(TREND_ORDER_SIZE) - find(TREND_ORDER, TREND_ORDER_SIZE, TREND.toInt())) / float(TREND_ORDER_SIZE);

  setupAndStartOTA();
}

void updateTrend() {
  currentPercent = currentPercent + 0.01;
  if (currentPercent > 1.0) {
    currentPercent = -currentPercent;
  }
  mTrend.setColor(min(maxPercent, abs(currentPercent)));
}

void loop() {
  if (needsReset) reset();

  if (millis() > nextTrendUpdate) {
    updateTrend();
    nextTrendUpdate += (TREND_UPDATE_PERIOD_MILLIS + 1e3L * random(0, 2));
  }

  if (millis() > nextBinaryUpdate) {
    checkForNewBinary();
    nextBinaryUpdate += (BINARY_UPDATE_PERIOD_MILLIS + 1e3L * random(-10, 10));
  }

  digitalWrite(2, (nextTrendUpdate / TREND_UPDATE_PERIOD_MILLIS) % 2);
  ArduinoOTA.handle();
}
