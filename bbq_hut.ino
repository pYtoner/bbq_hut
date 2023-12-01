
#include <FastLED.h>

#define LED_PIN 6
#define NUM_LEDS 170 * 6 + 1// actual number + 1 -> extra is used for out of bounds indexed setting
#define MAX_BRIGHTNESS 255

enum IndexingType {
  NORMAL,
  REVERSE,
  MIDDLE,
  MIDDLE_TOP,
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

void setup() {
  FastLED.addLeds<WS2812B, 6, GRB>(leds, NUM_LEDS);
  setAllBlack();
  delay(20);
}

void loop() {
  // CRGB color = CRGB(255, 255, 255);
  // setTapestry(0, color);
  // delay(50);
  // setTapestry(1, color);
  // delay(50);
  // setTapestry(2, color);
  // delay(50);
  // setTapestry(3, color);
  // delay(50);
  // setTapestry(4, color);
  // delay(50);
  // setTapestry(5, color);
  // delay(50);

  // rainbowPattern(MIDDLE);

  // messageSenderHSV(hueChange, 5, MIDDLE);
  // messageSenderHSV(hueChangeIndexed, 5, MIDDLE);
  // messageSenderHSV(hueLerp, 1, MIDDLE);

  // messageSenderRGB(twoColorLerpLCH, 1, MIDDLE);
  messageSenderRGB(hueChangeIndexedRGB, 1, MIDDLE);
}

void rainbowPattern(IndexingType indexing) {
  #define N_MESSAGES 85

  CHSV colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = CHSV(100, 255, 255);
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
    newColor.hue = hue;

    colors[0] = newColor;

    for (int i = 0; i <= N_MESSAGES - 1; i++) {
      for (int t = 0; t <= 6 - 1; t++) {
        leds[index(t, i, MIDDLE)] = colors[i];
        leds[index(t, 170-i, MIDDLE)] = colors[i];
      }
    }
    FastLED.show();
    delay(40);
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
  int hue = changeIdx * 20;
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

typedef CHSV (*FunctionPointerHSV)(CHSV, int);

// message delay should be 1 or more. 1 means message is sent every step
void messageSenderHSV(int (*changeFunctionHSV)(CHSV, int), int messageDelay, IndexingType indexing) {
  FunctionPointerHSV changeFunction = (FunctionPointerHSV)changeFunctionHSV;

  #define N_MESSAGES 85

  CHSV colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = CHSV(100, 255, 255);
  }

  int changeIdx = 0;
  while (true) {
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
  double f_l = 90;
  double f_c = 42.0;
  double f_h = 153.0;

  double t_l = 90;
  double t_c = 71.0;
  double t_h = 110.0;

  int speed = 5;

  int fract = (changeIdx * speed) % 511 - 255;
  if (fract < 0) {
    fract *= -1;
  }
  // double l = lerpDouble(f_l, t_l, fract);
  double c = lerpDouble(f_c, t_c, fract);
  // double h = lerpDouble(f_h, t_h, fract);

  // return LCHtoRGB(90, c, 120);
  return LCHtoRGB(90, f_c, 120);
  // return LCHtoRGB(90, t_c, 120);
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

typedef CRGB (*FunctionPointerRGB)(CRGB, int);

// message delay should be 1 or more. 1 means message is sent every step
void messageSenderRGB(int (*changeFunctionRGB)(CRGB, int), int messageDelay, IndexingType indexing) {
  FunctionPointerRGB changeFunction = (FunctionPointerRGB)changeFunctionRGB;

  #define N_MESSAGES 85

  CRGB colors[N_MESSAGES];

  for (int i = 0; i <= N_MESSAGES - 1; i++) {
    colors[i] = CRGB::Blue;
  }

  int changeIdx = 0;
  while (true) {
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
