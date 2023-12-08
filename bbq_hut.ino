// https://www.selecolor.com/en/hsv-color-picker/
// but it goes to 255 in CHSV for S and V

#include <FastLED.h>

#define BUTTON_PIN 2
#define LED_PIN 6
#define NUM_LEDS 1007 + 1 // actual number + 1 -> extra is used for out of bounds indexed setting
#define MAX_BRIGHTNESS 255

typedef CHSV (*FunctionPointerHSV)(CHSV, int);
typedef CRGB (*FunctionPointerRGB)(CRGB, int);

enum IndexingType {
  NORMAL,
  REVERSE,
  MIDDLE,
  MIDDLE_TOP,
};

enum Side {
  LEFT,
  TOP,
  RIGHT,
  BOTTOM,
};

enum Direction {
  FORWARDS,
  BACKWARDS,
};

CRGB leds[NUM_LEDS];

int nLeds[6] = {
  168,
  168,
  168,
  168,
  167,
  168,
};

int sideLengths[4] = {
    54, 10, 54, 50,
};

int modeIdx = 0;
int nModes = 8;

void setup(void) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, 6, GRB>(leds, NUM_LEDS);
  setAllBlack();
  delay(20);
}

void loop(void) {
  // Check for keypress
  if (!digitalRead(BUTTON_PIN)) {
    delay(100);

    // wait for it to turn off
    while (!digitalRead(BUTTON_PIN)) {
      // spin loop
      delay(1);
    }

    modeIdx += 1;
    modeIdx %= nModes;
  }

  switch (modeIdx) {
    case 0:
      setAllBlack();
      break;
    case 1:
      messageSenderHSV(hueChange, 1, MIDDLE, CHSV(235, 255, 255));
      break;
    case 2:
      messageSenderHSV(hueChangeIndexed, 5, MIDDLE, CHSV(0, 0, 0));
      break;
    case 3:
      messageSenderHSV(hueLerp, 1, MIDDLE, CHSV(0, 0, 0));
      break;
    case 4:
      messageSenderRGB(twoColorLerpLCH, 1, MIDDLE, CRGB(0, 0, 0));
      break; 
    case 5:
      messageSenderRGB(hueChangeIndexedRGB, 1, MIDDLE, CRGB(0, 0, 0));
      break;
    case 6:
      circlingDot();
      break;
    case 7:
      circlingPulse();
      break;
  }
}

CHSV hueLerp(CHSV oldColor, int changeIdx) {
  int fromHue = 120;
  int toHue = 155;

  int speed = 10;

  int fract = (changeIdx * speed) % 511 - 255;
  if (fract < 0) {
    fract *= -1;
  }
  int hue = lerp8by8(fromHue, toHue, fract);

  return CHSV(hue, 255, 255);
}

CHSV hueChangeIndexed(CHSV oldColor, int changeIdx) {
  int speed = 20;
  int hue = changeIdx * speed;
  hue %= 255;

  return CHSV(hue, 255, 255);
}

CHSV hueChange(CHSV oldColor, int changeIdx) {
  int hue = oldColor.hue;
  hue += 20;
  hue %= 255;

  CHSV newColor = oldColor;
  newColor.hue = hue;

  return newColor;
}

// message delay should be 1 or more. 1 means message is sent every step
void messageSenderHSV(FunctionPointerHSV changeFunction, int messageDelay, IndexingType indexing, CHSV startColor) {
  #define N_MESSAGES 85

  CHSV colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = startColor;
  }

  int changeIdx = 0;
  while (true) {
    if (isTryingToSwitchMode()) {
      break;
    }

    for (int i = N_MESSAGES; i > 0; i--) {
      colors[i] = colors[(i + N_MESSAGES - 1) % N_MESSAGES];
    }

    colors[0] = changeFunction(colors[messageDelay], changeIdx / messageDelay);

    for (int i = 0; i <= N_MESSAGES - 1; i++) {
      for (int t = 0; t <= 6 - 1; t++) {
        leds[index(t, i, indexing)] = colors[i];
        leds[index(t, 170-i, indexing)] = colors[i];
      }
    }
    FastLED.show();
    delay(40);

    changeIdx += 1;
  }
}

CRGB twoColorLerpLCH(CRGB oldColor, int changeIdx) {
  double f_l = 53.62576010848221;
  double f_c = 58.118145058923176;
  double f_h = 51.33352103353704;

  double t_l = 43.18878705912335;
  double t_c = 60.471578319098455;
  double t_h = 284.33499171833785;

  int speed = 10;

  int fract = (changeIdx * speed) % 511 - 255;
  if (fract < 0) {
    fract *= -1;
  }
  double l = lerpDouble(f_l, t_l, fract);
  double c = lerpDouble(f_c, t_c, fract);
  double h = lerpDouble(f_h, t_h, fract);

  return LCHtoRGB(l, c, h);
}

CRGB hueChangeIndexedRGB(CRGB oldColor, int changeIdx) {
  int speed = 20;

  int fract = (changeIdx * speed) % 511 - 255;
  if (fract < 0) {
    fract *= -1;
  }

  double normalized = (double)fract / 255.0;
  double transformed = pow(normalized, 4.0) * 255.0;

  return CRGB((int)transformed, 0, 0);
}

double lerpDouble(double from, double to, int fract) {
  return from + (( to - from ) * (double)fract) / 256;
}

// message delay should be 1 or more. 1 means message is sent every step
void messageSenderRGB(FunctionPointerRGB changeFunction, int messageDelay, IndexingType indexing, CRGB startColor) {
  #define N_MESSAGES 85

  CRGB colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = startColor;
  }

  int changeIdx = 0;
  while (true) {
    if (isTryingToSwitchMode()) {
      break;
    }

    for (int i = N_MESSAGES; i > 0; i--) {
      colors[i] = colors[(i + N_MESSAGES - 1) % N_MESSAGES];
    }

    colors[0] = changeFunction(colors[messageDelay], changeIdx / messageDelay);

    for (int i = 0; i <= N_MESSAGES - 1; i++) {
      for (int t = 0; t <= 6 - 1; t++) {
        leds[index(t, i, indexing)] = colors[i];
        leds[index(t, 170-i, indexing)] = colors[i];
      }
    }
    FastLED.show();
    delay(40);

    changeIdx += 1;
  }
}

void setTapestryBlack(int tapestryIdx) {
  setTapestry(tapestryIdx, CRGB(0, 0, 0));
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
      return padding + idx;
    case REVERSE:
      return padding + (nLeds[tapestryIdx] - idx);
    case MIDDLE:
      return padding + (idx + offsetForBottomMiddle[tapestryIdx]) % nLeds[tapestryIdx];
    case MIDDLE_TOP:
      return padding + (idx + offsetForTopMiddle[tapestryIdx]) % nLeds[tapestryIdx];
    default:
      return padding + idx; // same as normal indexing
  }
}

int sideIndex(int tapestryIdx, int idx, Side side, Direction direction) {
  int tapestryPadding = 0;
  for (int i = 0; i <= tapestryIdx - 1; i++) {
    tapestryPadding += nLeds[i];
  }

  int sidePadding = 0;
  for (int i = 0; i <= (int)side - 1; i++) {
    sidePadding += sideLengths[i];
  }

  // return dummy led if out of bounds
  if (idx >= sideLengths[(int)side]) {
    return NUM_LEDS;
  }

  if (direction == FORWARDS) {
    return tapestryPadding + sidePadding + idx;
  } else {
    return tapestryPadding + sidePadding + (sideLengths[(int)side] - idx);
  }
}

int circularIndex(int idx, Side side) {
  int tapestry_idx = idx / sideLengths[(int)side];

  int side_idx = idx % sideLengths[(int)side];

  int transformed_idx = sideIndex(tapestry_idx, side_idx, side, FORWARDS);
  return transformed_idx;
}

// pulse(i % 10, 5)
// min value will be 0 max will be 5
int pulse(int n, int max_val) {
  if (n > max_val) {
    n -= max_val * 2;
    n *= -1;
  }

  return n;
}

void circlingDot() {
  int changeIdx = 0;
  while (true) {
    if (isTryingToSwitchMode()) {
      break;
    }

    setAllBlack();

    // 0 to 1
    double pattern_progress = (double)(changeIdx % 100) / 100.0;

    double tapestry_progress = fmod(pattern_progress * 6.0, 1.0);
    int tapestry_idx = (int)(pattern_progress * 6);

    {
      int side = 1;
      int side_idx = (int)(tapestry_progress * sideLengths[side]);
      int transformed_idx = sideIndex(tapestry_idx, side_idx, side, 0);
      leds[transformed_idx] = CRGB(255, 0, 0);
    }

    {
      int side = 3;
      int side_idx = (int)(tapestry_progress * sideLengths[side]);
      int transformed_idx = sideIndex(tapestry_idx, side_idx, side, 0);
      leds[transformed_idx] = CRGB(255, 0, 0);
    }

    changeIdx += 1;
  }
}

void circlingPulse() {
  int changeIdx = 0;
  while (true) {
    if (isTryingToSwitchMode()) {
      break;
    }

    setAllBlack();

    int side = LEFT;
    int maximum = sideLengths[(int)side] * 6;

    int n_dots = 20;
    for (int t = 0; t <= n_dots - 1; t++) {
      int i = (changeIdx + t) % maximum;
      int transformed_idx = circularIndex(i, side);

      double intensity = (double)pulse(t, n_dots / 2) / ((double)n_dots / 2.0);
      int color = (int)(255.0 * powf(intensity, 1.3));
      leds[transformed_idx] = CRGB(color, 0, 0);
    }
    changeIdx += 1;
  }
}

bool isTryingToSwitchMode() {
  return !digitalRead(BUTTON_PIN);
}

//////////////////
/// LCH TO RGB ///
//////////////////
void _LCHtoLab(double L, double C, double H, double *l, double *a, double *b) {
  *l = L;
  *a = cos(H * M_PI / 180.0) * C;
  *b = sin(H * M_PI / 180.0) * C;
}

// Function to convert Lab to XYZ
void _LabToXYZ(double L, double A, double B, double *X, double *Y, double *Z) {
    double epsilon = 0.008856;  // Intent for use in a function or algorithm
    double kappa = 903.3;       // Intent for use in a function or algorithm
    double Xn = 0.95047;        // Assuming D65 illuminant
    double Yn = 1.00000;        // Assuming D65 illuminant
    double Zn = 1.08883;        // Assuming D65 illuminant

    *Y = L > (kappa * epsilon) ? pow((L + 16) / 116, 3) : L / kappa;
    double fY = L > (kappa * epsilon) ? pow(*Y, 1.0/3.0) : (kappa * *Y + 16) / 116;
    double fX = A / 500 + fY;
    double fZ = fY - B / 200;

    *X = pow(fX, 3) > epsilon ? pow(fX, 3) : (116 * fX - 16) / kappa;
    *Z = pow(fZ, 3) > epsilon ? pow(fZ, 3) : (116 * fZ - 16) / kappa;

    *X *= Xn;
    *Y *= Yn;
    *Z *= Zn;
}

// Function to clamp RGB values
double _clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Function to apply gamma correction
double _gammaCorrect(double value) {
    if (value <= 0.0031308) {
        return 12.92 * value;
    } else {
        return 1.055 * pow(value, 1.0 / 2.4) - 0.055;
    }
}

// Function to convert XYZ to RGB
CRGB _XYZtoRGB(double X, double Y, double Z) {
    // Assuming sRGB color space and D65 white point
    double R =  3.2406 * X - 1.5372 * Y - 0.4986 * Z;
    double G = -0.9689 * X + 1.8758 * Y + 0.0415 * Z;
    double B =  0.0557 * X - 0.2040 * Y + 1.0570 * Z;

    // Clamp values to the 0-1 range
    R = _clamp(R, 0.0, 1.0);
    G = _clamp(G, 0.0, 1.0);
    B = _clamp(B, 0.0, 1.0);

    // Apply gamma correction
    R = _gammaCorrect(R);
    G = _gammaCorrect(G);
    B = _gammaCorrect(B);

    return CRGB((int)(R * 255), (int)(G * 255), (int)(B * 255));
}

// L darkness: black 0-100 white
// C intensity: ?
// H hue: 0-360
CRGB LCHtoRGB(double L, double C, double H) {
  double l, a, b;
  double X, Y, Z;
  _LCHtoLab(L, C, H, &l, &a, &b);
  _LabToXYZ(l, a, b, &X, &Y, &Z);
  return _XYZtoRGB(X, Y, Z);
}