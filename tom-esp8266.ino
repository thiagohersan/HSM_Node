#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#define TEST 0

#include "Trend.h"
#include "wifipass.h"

#if TEST
String ENDPOINT = "https://www.random.org/integers/";
String TREND = "?num=1&min=0&max=100&col=1&base=10&format=plain&rnd=today";
#else
String ENDPOINT = "http://10.10.119.10:3000/panel/cubes/";
String TREND = "1";
#endif

double SLEEP = 30e6;

Trend mTrend;

void update() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(ENDPOINT + TREND);
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
  pinMode(2, OUTPUT);
  WiFi.begin(WIFI, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  digitalWrite(2, LOW);
  update();
  digitalWrite(2, HIGH);
  ESP.deepSleep(SLEEP);
}

void loop() {}

