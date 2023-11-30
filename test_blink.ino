#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 170 + 170 + 110
#define MAX_BRIGHTNESS 255

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, 6, GRB>(leds, NUM_LEDS);
  // setAllBlack(0);
  // delay(20);
  // setAllBlack(1);
  // delay(20);
  // setAllBlack(2);
  // delay(20);

  
}

void loop() {
  setAll(0, CRGB(255, 0, 0));
  delay(100);

  rainbowUpPattern();
}

void rainbowUpPattern() {
  CRGB colors[85];
  CRGB shifted[85];

  for (int i = 0; i <= 85 - 1; i++) {
    colors[i] = CRGB(255, 0, 0);
  }

  for (int i = 0; i <= 85 - 1; i++) {
    shifted[i] = CRGB(255, 0, 0);
  }

  while (true) {
    for (int i = 0; i <= 85 - 1; i++) {
      shifted[i] = colors[(i + 84) % 85];

      // change color
      CHSV newColor = rgb2hsv_approximate(shifted[i]);
      int hue = newColor.hue;
      hue += 20;
      hue %= 255;
      shifted[83].setHue(hue);
    }
  
    for (int i = 0; i <= 170 - 1; i++) {
      colors[i] = shifted[i];
    }

    for (int i = 0; i <= 85 - 1; i++) {
      leds[middleIndex(i)] = colors[i];
      leds[middleIndex(170-i)] = colors[i];
      leds[170+middleIndex(i)] = colors[i];
      leds[170+middleIndex(170-i)] = colors[i];
    
      leds[170+170+i] = colors[i];
    }
    FastLED.show();
    delay(40);
  }
}

void rainbowDownPattern() {
  CRGB colors[85];
  CRGB shifted[85];

  for (int i = 0; i <= 85 - 1; i++) {
    colors[i] = CRGB(255, 0, 0);
  }

  for (int i = 0; i <= 85 - 1; i++) {
    shifted[i] = CRGB(255, 0, 0);
  }

  while (true) {
    for (int i = 0; i <= 85 - 1; i++) {
      shifted[i] = colors[(i + 1) % 85];

      // change color
      CHSV newColor = rgb2hsv_approximate(shifted[i]);
      int hue = newColor.hue;
      hue += 20;
      hue %= 255;
      shifted[5].setHue(hue);
    }
  
    for (int i = 0; i <= 170 - 1; i++) {
      colors[i] = shifted[i];
    }

    for (int i = 0; i <= 85 - 1; i++) {
      leds[middleIndex(i)] = colors[i];
      leds[middleIndex(170-i)] = colors[i];
    }
    FastLED.show();
    delay(40);
  }
}

int middleIndex(int idx) {
  return (idx + 170 - 27) % 170;
}

void setAllBlack(int tapestryIdx) {
  setAll(tapestryIdx, CRGB(0, 0, 0));
}

void setAll(int tapestryIdx, CRGB color) {
  for (int i = 0; i <= 170 - 1; i++) {
    leds[index(tapestryIdx, i)] = color;
  }
  FastLED.show();
}

int index(int tapestryIdx, int idx) {
  return tapestryIdx * 170 + idx;
}