#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define NUMPIXELS 16

class Trend {
  private:
    float lerpPosition;
    static uint32_t temperatureColors[];
    uint32_t lerpColors[2] = { 0x0, 0x0 };
    Adafruit_NeoPixel *mPixels;
    uint32_t lerpColor(uint32_t c0, uint32_t c1, float pos);
    float lerp(float y0, float y1, float pos);
    void update();
  public:
    Trend();
    void setColor(float percent);
};


