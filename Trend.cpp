#include "Trend.h"

uint32_t Trend::temperatureColors[] = { 0x36a9f6, 0x5f00cc, 0xff2323 };

Trend::Trend() {}

void Trend::setColor(float percent) {
  if (percent < 0.5) {
    mColor = lerpColor(temperatureColors[0], temperatureColors[1], 2 * percent);
  } else {
    mColor = lerpColor(temperatureColors[1], temperatureColors[2], 2 * (percent - 0.5));
  }

  mPixels = new Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
  mPixels->begin();

  for (int i = 0; i < NUMPIXELS; i++) {
    mPixels->setPixelColor(i, mColor);
  }
  mPixels->setBrightness(constrain(percent * 255.0, 32, 255));

  mPixels->show();
  delay(15);
  delete mPixels;
}

uint32_t Trend::lerpColor(uint32_t c0, uint32_t c1, float pos) {
  pos = constrain(pos, 0.0, 1.0);
  return Adafruit_NeoPixel::Color((uint8_t) lerp((c0 >> 16) & 0xff, (c1 >> 16) & 0xff, pos),
                                  (uint8_t) lerp((c0 >> 8) & 0xff, (c1 >> 8) & 0xff, pos),
                                  (uint8_t) lerp((c0 >> 0) & 0xff, (c1 >> 0) & 0xff, pos));
}

float Trend::lerp(float y0, float y1, float pos) {
  pos = constrain(pos, 0.0, 1.0);
  return y0 + pos * (y1 - y0);
}

