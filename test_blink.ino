#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 170 * 6 // actual number + 1 -> extra is used for out of bounds indexed setting
#define MAX_BRIGHTNESS 255

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
        leds[middleTopIndex(0, i)] = colors[i];
        leds[middleTopIndex(0, 170-i)] = colors[i];
        leds[middleTopIndex(1, i)] = colors[i];
        leds[middleTopIndex(1, 170-i)] = colors[i];
        leds[middleTopIndex(2, i)] = colors[i];
        leds[middleTopIndex(2, 170-i)] = colors[i];
      } else {
        leds[middleIndex(0, i)] = colors[i];
        leds[middleIndex(0, 170-i)] = colors[i];
        leds[middleIndex(1, i)] = colors[i];
        leds[middleIndex(1, 170-i)] = colors[i];
        leds[middleIndex(2, i)] = colors[i];
        leds[middleIndex(2, 170-i)] = colors[i];
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
    leds[index(tapestryIdx, i)] = color;
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

// indexing starting from bottom middle
int middleIndex(int tapestryIdx, int idx) {
  int padding = 0;

  for (int i = 0; i <= tapestryIdx - 1; i++) {
    padding += nLeds[i];
  }

  return padding + (idx + offsetForBottomMiddle[tapestryIdx]) % nLeds[tapestryIdx];
}

int offsetForTopMiddle[6] = {
  70,
  70,
  70,
  70,
  70,
  70,
};

// indexing starting from top middle
int topMiddleIndex(int tapestryIdx, int idx) {
  int padding = 0;

  for (int i = 0; i <= tapestryIdx - 1; i++) {
    padding += nLeds[i];
  }

  return padding + (idx + offsetForTopMiddle[tapestryIdx]) % nLeds[tapestryIdx];
}

// wraps around the tapestry if idx is more than the n leds in that tapestry
int index(int tapestryIdx, int idx) {
  int padding = 0;

  for (int i = 0; i <= tapestryIdx - 1; i++) {
    padding += nLeds[i];
  }

  // return dummy led if out of bounds
  if (idx >= nLeds[tapestryIdx]) {
    return NUM_LEDS;
  }

  return padding + idx;
}
