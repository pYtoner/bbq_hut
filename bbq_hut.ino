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

  rainbowPattern(MIDDLE);
}

void rainbowPattern(IndexingType indexing) {
  #define N_MESSAGES 85

  CHSV colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = CHSV::Blue;
  }

  while (true) {
    for (int i = N_MESSAGES; i > 0; i--) {
      colors[i] = colors[(i + N_MESSAGES - 1) % N_MESSAGES];
    }

    // change color
    CHSV newColor = colors[1];
    int hue = newColor.hue;
    hue += 20;
    hue %= 255;

    colors[0] = newColor;

    for (int i = 0; i <= N_MESSAGES - 1; i++) {
      for (int t = 0; t <= 6 - 1; t++) {
        leds[index(t, i), indexing] = colors[i];
        leds[index(t, 170-i), indexing] = colors[i];
      }
    }
    FastLED.show();
    delay(40);
  }
}

void messageSender() {
  #define N_MESSAGES 85

  CHSV colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = CHSV::Blue;
  }

  while (true) {
    for (int i = N_MESSAGES; i > 0; i--) {
      colors[i] = colors[(i + N_MESSAGES - 1) % N_MESSAGES];
    }

    // change color
    CHSV newColor = colors[1];
    int hue = newColor.hue;
    hue += 20;
    hue %= 255;

    colors[0] = newColor;

    for (int i = 0; i <= N_MESSAGES - 1; i++) {
      if isUpwards {
        leds[index(0, i), MIDDLE] = colors[i];
        leds[index(0, 170-i), MIDDLE] = colors[i];
        leds[index(1, i), MIDDLE] = colors[i];
        leds[index(1, 170-i), MIDDLE] = colors[i];
        leds[index(2, i), MIDDLE] = colors[i];
        leds[index(2, 170-i), MIDDLE] = colors[i];
      } else {
        leds[index(0, i, MIDDLE_TOP)] = colors[i];
        leds[index(0, 170-i), MIDDLE_TOP] = colors[i];
        leds[index(1, i), MIDDLE_TOP] = colors[i];
        leds[index(1, 170-i), MIDDLE_TOP] = colors[i];
        leds[index(2, i), MIDDLE_TOP] = colors[i];
        leds[index(2, 170-i), MIDDLE_TOP] = colors[i];
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
