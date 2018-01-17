#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#include "Trend.h"
#include "wifipass.h"

int TREND_ORDER[] = {7, 12, 18, 8, 4, 21, 24, 22, 16, 2, 17, 11, 9, 10, 23, 5, 3, 1, 15, 20, 13, 6, 14, 19};
int TREND_ORDER_SIZE = 24;

int TREND = 19;
String HOSTNAME = "ToM-";

int SLEEP_MILLIS = 30e3;

Trend mTrend;
long nextUpdate;

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);

  nextUpdate = 0;
  HOSTNAME += TREND;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  ArduinoOTA.setHostname(HOSTNAME.c_str());
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

int find(int arr[], int arrSize, int item) {
  for (int i = 0; i < arrSize; i++) {
    if (arr[i] == item) {
      return i;
    }
  }
  return arrSize;
}

void loop() {
  if (millis() > nextUpdate) {
    int found_index = find(TREND_ORDER, TREND_ORDER_SIZE, TREND);
    if (found_index <= TREND_ORDER_SIZE) {
      float colorPercent = (float(TREND_ORDER_SIZE) - found_index) / float(TREND_ORDER_SIZE);
      mTrend.setColor(colorPercent);
    }
    nextUpdate += SLEEP_MILLIS;
  }
  digitalWrite(2, ((nextUpdate / SLEEP_MILLIS) % 2));

  ArduinoOTA.handle();
}

