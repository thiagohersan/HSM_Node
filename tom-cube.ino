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
int TREND_ORDER_SIZE = sizeof(TREND_ORDER)/sizeof(TREND_ORDER[0]);

long SLEEP_MILLIS = 30e3L;
long nextUpdate = 0L;

Trend mTrend;

void update() {
  int found_index = find(TREND_ORDER, TREND.toInt());
  if (found_index <= TREND_ORDER_SIZE) {
    float colorPercent = (float(TREND_ORDER_SIZE) - found_index) / float(TREND_ORDER_SIZE);
    mTrend.setColor(colorPercent);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(2, OUTPUT);

  nextUpdate = millis() + SLEEP_MILLIS;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  TREND = WiFi.localIP()[3];
  OTA_HOSTNAME += TREND;

  setupAndStartOTA();
  checkForNewBinary();
}

void loop() {
  if (needsReset) reset();

  if (millis() > nextUpdate) {
    update();
    checkForNewBinary();
    nextUpdate += SLEEP_MILLIS;
  }
  digitalWrite(2, ((nextUpdate / SLEEP_MILLIS / 30) % 2));

  ArduinoOTA.handle();
}

