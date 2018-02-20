#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define NUMPIXELS 16

class Trend {
  private:
    uint32_t mColor;
    void setLedColorsAndBrightness(float colorPercent, float brightnessPercent);
    static uint32_t temperatureColors[];
    Adafruit_NeoPixel *mPixels;
    uint32_t lerpColor(uint32_t c0, uint32_t c1, float pos);
    float lerp(float y0, float y1, float pos);
  public:
    Trend();
    void setColor(float colorPercent);
    void sweepColor(float maxColorPercent);
};


