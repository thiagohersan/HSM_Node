#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <Adafruit_NeoPixel.h>

#define TEST 1

#include "Trend.h"
#include "wifipass.h"
#include "parameters.h"

String SERVER_ADDRESS = "10.75.124.30";
int SERVER_PORT = 8000;
String ENDPOINT_BINARY = "/bin/" + BINARY_VERSION + "/" + TREND;
String ENDPOINT_CUBE = "/panel/cubes/" + TREND;

int SLEEP_MILLIS = 5e3;
long nextUpdate;
bool needsReset = false;

Trend mTrend;

void update() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://" + SERVER_ADDRESS + ":" + SERVER_PORT + ENDPOINT_CUBE);
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


  t_httpUpdate_return ret = ESPhttpUpdate.update(SERVER_ADDRESS, SERVER_PORT, ENDPOINT_BINARY, "", false, "", false);
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

