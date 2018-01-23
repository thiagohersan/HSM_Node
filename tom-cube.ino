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

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(2, OUTPUT);

  randomSeed(analogRead(A0));
  nextUpdate = millis() + (UPDATE_PERIOD_MILLIS + 1e3L * random(-10, 10));

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  TREND = WiFi.localIP()[3];
  OTA_HOSTNAME += TREND;

  setupAndStartOTA();
}

void updateTrend() {
  int found_index = find(TREND_ORDER, TREND.toInt());
  if (found_index <= TREND_ORDER_SIZE) {
    float colorPercent = (float(TREND_ORDER_SIZE) - found_index) / float(TREND_ORDER_SIZE);
    mTrend.setColor(colorPercent);
  }
}

void loop() {
  if (needsReset) reset();

  if (millis() > nextUpdate) {
    updateTrend();
    checkForNewBinary();
    nextUpdate += (UPDATE_PERIOD_MILLIS + 1e3L * random(-10, 10));
  }
  digitalWrite(2, ((millis() / 500) % 2));

  ArduinoOTA.handle();
}

