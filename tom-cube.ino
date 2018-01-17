#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <Adafruit_NeoPixel.h>

#define TEST 1

#include "Trend.h"
#include "wifipass.h"
#include "parameters.h"

String BINARY_SERVER_ADDRESS = "10.75.124.30";
int BINARY_SERVER_PORT = 8000;
String BINARY_SERVER_ENDPOINT = "/bin/" + BINARY_VERSION + "/" + TREND;

String TREND_SERVER_ADDRESS = "10.75.124.30";
int TREND_SERVER_PORT = 8000;
String TREND_SERVER_ENDPOINT = "/panel/cubes/" + TREND;

int SLEEP_MILLIS = 5e3;
long nextUpdate;
bool needsReset = false;

Trend mTrend;

void update() {
  if (WiFi.status() == WL_CONNECTED) {
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
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n");
  pinMode(2, OUTPUT);

  mTrend.setColor(0.0);
  nextUpdate = millis() + SLEEP_MILLIS;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }


  t_httpUpdate_return ret = ESPhttpUpdate.update(BINARY_SERVER_ADDRESS, BINARY_SERVER_PORT, BINARY_SERVER_ENDPOINT, "", false, "", false);
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.println("[UPDATE] failed.");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[UPDATE] NO update.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("[UPDATE] OK.");
      needsReset = true;
      break;
  }
}

void reset() {
  mTrend.setColor(0);
  delay(1000);
  ESP.deepSleep(500e3);
}

void loop() {
  if (needsReset) reset();

  if (millis() > nextUpdate) {
    update();
    nextUpdate += SLEEP_MILLIS;
  }
  digitalWrite(2, (nextUpdate / SLEEP_MILLIS) % 2);
}

