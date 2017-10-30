#include "Trend.h"

uint32_t Trend::allColors[] = { Adafruit_NeoPixel::Color(111, 40, 104),
                                Adafruit_NeoPixel::Color(62, 71, 220),
                                Adafruit_NeoPixel::Color(47, 93, 183),
                                Adafruit_NeoPixel::Color(20, 122, 188),
                                Adafruit_NeoPixel::Color(31, 113, 147),
                                Adafruit_NeoPixel::Color(15, 132, 112),
                                Adafruit_NeoPixel::Color(0, 153, 76),
                                Adafruit_NeoPixel::Color(52, 173, 62),
                                Adafruit_NeoPixel::Color(153, 215, 31),
                                Adafruit_NeoPixel::Color(203, 235, 15),
                                Adafruit_NeoPixel::Color(255, 248, 123),
                                Adafruit_NeoPixel::Color(255, 204, 1),
                                Adafruit_NeoPixel::Color(255, 152, 0),
                                Adafruit_NeoPixel::Color(255, 102, 1),
                                Adafruit_NeoPixel::Color(235, 52, 56),
                                Adafruit_NeoPixel::Color(255, 52, 0)
                              };

uint8_t Trend::numColors = sizeof(Trend::allColors) / sizeof(uint32_t);

Trend::Trend() {
  lerpPosition = 1.0;
}

void Trend::update() {
  if (lerpPosition >= 1.0) return;

  lerpPosition = constrain(lerpPosition + 0.05, 0.0, 1.0);
  mPixels = new Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
  mPixels->begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    mPixels->setPixelColor(i, lerpColor(lerpColors[0], lerpColors[1], lerpPosition));
  }
  mPixels->show();
  delay(15);
  delete mPixels;
}
void Trend::setColor(float percent) {
  lerpColors[0] = lerpColors[1];
  lerpColors[1] = allColors[constrain((int)(percent * numColors), 0, numColors - 1)];
  lerpPosition = 0.0;
}

uint32_t Trend::lerpColor(uint32_t c0, uint32_t c1, float pos) {
  return Adafruit_NeoPixel::Color((uint8_t) lerp((c0 >> 16) & 0xff, (c1 >> 16) & 0xff, pos),
                                  (uint8_t) lerp((c0 >> 8) & 0xff, (c1 >> 8) & 0xff, pos),
                                  (uint8_t) lerp((c0 >> 0) & 0xff, (c1 >> 0) & 0xff, pos));
}

float Trend::lerp(float y0, float y1, float pos) {
  pos = constrain(pos, 0.0, 1.0);
  return y0 + pos * (y1 - y0);
}

