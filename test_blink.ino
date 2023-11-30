#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 170 * 6 // actual number + 1 -> extra is used for out of bounds indexed setting
#define MAX_BRIGHTNESS 255

enum IndexingType {
  NORMAL,
  REVERSE,
  MIDDLE,
  MIDDLE_TOP,
}

CRGB leds[NUM_LEDS];

int nLeds[6] = {
  170,
  170,
  170,
  170,
  170,
  170,
};

void setup() {
  FastLED.addLeds<WS2812B, 6, GRB>(leds, NUM_LEDS);
  setAllBlack();
  delay(20);
}

void loop() {
  setAll(CRGB(255, 0, 0));
  delay(100);

  rainbowPattern(true);
}

void rainbowPattern(bool isUp) {
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
  
    for (int i = 0; i <= 85 - 1; i++) {
      colors[i] = shifted[i];
    }

    for (int i = 0; i <= 85 - 1; i++) {
      if isUp {
        leds[index(0, i, MIDDLE_TOP)] = colors[i];
        leds[index(0, 170-i), MIDDLE_TOP] = colors[i];
        leds[index(1, i), MIDDLE_TOP] = colors[i];
        leds[index(1, 170-i), MIDDLE_TOP] = colors[i];
        leds[index(2, i), MIDDLE_TOP] = colors[i];
        leds[index(2, 170-i), MIDDLE_TOP] = colors[i];
      } else {
        leds[index(0, i), MIDDLE] = colors[i];
        leds[index(0, 170-i), MIDDLE] = colors[i];
        leds[index(1, i), MIDDLE] = colors[i];
        leds[index(1, 170-i), MIDDLE] = colors[i];
        leds[index(2, i), MIDDLE] = colors[i];
        leds[index(2, 170-i), MIDDLE] = colors[i];
      }
    }
    FastLED.show();
    delay(40);
  }
}

void setTapestryBlack(int tapestryIdx) {
  setAll(tapestryIdx, CRGB(0, 0, 0));
}

void setTapestry(int tapestryIdx, CRGB color) {
  for (int i = 0; i <= 170 - 1; i++) {
    leds[index(tapestryIdx, i, NORMAL)] = color;
  }
  FastLED.show();
}

void setAllBlack() {
  for (int i = 0; i <= 6 - 1; i++) {
    setTapestryBlack(i);
  }
}

void setAll(CRGB color) {
  for (int i = 0; i <= 6 - 1; i++) {
    setTapestry(i, color);
  }
}

int offsetForBottomMiddle[6] = {
  143,
  143,
  143,
  143,
  143,
  143,
};

int offsetForTopMiddle[6] = {
  70,
  70,
  70,
  70,
  70,
  70,
};

// returns dummy led idx if out of bounds for the tapestry
int index(int tapestryIdx, int idx, IndexingType indexing) {
  int padding = 0;

  for (int i = 0; i <= tapestryIdx - 1; i++) {
    padding += nLeds[i];
  }

  // return dummy led if out of bounds
  if (idx >= nLeds[tapestryIdx]) {
    return NUM_LEDS;
  }

  switch (indexing) {
    case NORMAL:
      return padding + (nLeds[tapestryIdx] - idx);
    case REVERSE:
      return padding + idx;
    case MIDDLE:
      return padding + (idx + offsetForBottomMiddle[tapestryIdx]) % nLeds[tapestryIdx];
    case MIDDLE_TOP:
      return padding + (idx + offsetForTopMiddle[tapestryIdx]) % nLeds[tapestryIdx];
    default:
      return padding + idx; // same as normal indexing
  }
}
