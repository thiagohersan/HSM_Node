#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define NUMPIXELS 16

class Trend {
  private:
    float lerpPosition;
    uint32_t lerpColors[2] = { Adafruit_NeoPixel::Color(0, 0, 0), Adafruit_NeoPixel::Color(0, 0, 0) };
    uint32_t lerpColor(uint32_t c0, uint32_t c1, float pos);
    float lerp(float y0, float y1, float pos);
    Adafruit_NeoPixel *mPixels;
    static uint32_t temperatureColors[];
  public:
    Trend();
    void update();
    void setColor(float percent);
};


