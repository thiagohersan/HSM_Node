// board LED is connected to pin 2

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

#include "Trend.h"
#include "wifipass.h"

#define TREND 0
//#define ENDPOINT "https://trendometer-test.herokuapp.com/panel/cubes/"
#define ENDPOINT "https://www.random.org/integers/?num=1&min=0&max=100&col=1&base=10&format=plain&rnd=today"
#define NUM_TRENDS 24.0

Trend mTrend;

void update() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(ENDPOINT);
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
  ESP.deepSleep(20e6);
}

void loop() {}

