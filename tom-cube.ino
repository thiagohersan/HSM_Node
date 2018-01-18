#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#define TEST 1

#include "Trend.h"
#include "wifipass.h"
#include "parameters.h"

String OTA_HOSTNAME = "ToM-";

String BINARY_SERVER_ADDRESS = "10.75.124.30";
int BINARY_SERVER_PORT = 8000;
String BINARY_SERVER_ENDPOINT = "/bin/" + BINARY_VERSION + "/" + TREND;

String TREND_SERVER_ADDRESS = "10.75.124.30";
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

void reset() {
  ESP.deepSleep(500e3);
}

void checkForNewBinary() {
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

void setupAndStartOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME.c_str());
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

