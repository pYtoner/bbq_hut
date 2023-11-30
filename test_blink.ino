#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 170 * 6
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
  
    for (int i = 0; i <= 85 - 1; i++) {
      colors[i] = shifted[i];
    }

    for (int i = 0; i <= 85 - 1; i++) {
      leds[middleIndex(0, i)] = colors[i];
      leds[middleIndex(0, 170-i)] = colors[i];
      leds[middleIndex(1, i)] = colors[i];
      leds[middleIndex(1, 170-i)] = colors[i];
      leds[middleIndex(2, i)] = colors[i];
      leds[middleIndex(2, 170-i)] = colors[i];
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
  
    for (int i = 0; i <= 85 - 1; i++) {
      colors[i] = shifted[i];
    }

    for (int i = 0; i <= 85 - 1; i++) {
      leds[middleIndex(0, i)] = colors[i];
      leds[middleIndex(0, 170-i)] = colors[i];
      leds[middleIndex(1, i)] = colors[i];
      leds[middleIndex(1, 170-i)] = colors[i];
      leds[middleIndex(2, i)] = colors[i];
      leds[middleIndex(2, 170-i)] = colors[i];
    }
    FastLED.show();
    delay(40);
  }
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

  return padding + idx % nLeds[tapestryIdx];
}
