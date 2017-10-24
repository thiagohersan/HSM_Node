#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#include "Trend.h"
#include "wifipass.h"

#define TREND "iot"
#define NUM_TRENDS 24.0

Trend mTrend;
long lastGet;

void setup() {
  lastGet = millis();
  WiFi.begin(WIFI, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  mTrend.update();

  if ((millis() - lastGet > 5000) && (WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("https://www.random.org/integers/?num=1&min=0&max=23&col=1&base=10&format=plain&rnd=today");
    int httpCode = http.GET();
    delay(10);

    if (httpCode == HTTP_CODE_OK) {
      float rank = http.getString().toFloat();
      lastGet = millis();
      mTrend.setColor(rank / NUM_TRENDS);
    }
    http.end();
  }
}

