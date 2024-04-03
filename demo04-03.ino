#include <PulseSensorPlayground.h>
#include <Adafruit_NeoPixel.h>

const int PULSE_INPUT = A0;
const int PULSE_BLINK = LED_BUILTIN;
const int PULSE_FADE = 5;
const int THRESHOLD = 580;

const int PIN_SPEAKER = 3;

const int LED_UPDATE_INTERVAL = 20;
unsigned long lastLedUpdate;
int ledFadeDirection = 1;
int ledGradientStep = 0;

unsigned int lastBeatDetected;
const unsigned long BEAT_FADE_TIME = 1600;

// NeoPixel setup
#define PIN_STRIP1 10
#define PIN_STRIP2 8
#define PIN_STRIP3 9
#define PIN_STRIP4 13
#define NUMPIXELS  6 // Assuming both strips have the same number of pixels

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS, PIN_STRIP1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS, PIN_STRIP2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUMPIXELS, PIN_STRIP3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUMPIXELS, PIN_STRIP4, NEO_GRB + NEO_KHZ800);

PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(115200);

  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(SERIAL_PLOTTER);
  pulseSensor.setThreshold(THRESHOLD);

  pinMode(PIN_SPEAKER, OUTPUT);

  strip1.begin(); // Initialize the first strip
  strip2.begin(); // Initialize the second strip
  strip3.begin(); // Initialize the third strip
  strip4.begin(); // Initialize the fourth strip
  strip1.show();  // Turn off all pixels ASAP
  strip2.show();  // Turn off all pixels ASAP
  strip3.show();  // Turn off all pixels ASAP
  strip4.show();  // Turn off all pixels ASAP

  lastLedUpdate = millis();
  lastBeatDetected = millis() - BEAT_FADE_TIME;

  if (!pulseSensor.begin()) {
    while (true) {
      digitalWrite(PULSE_BLINK, LOW);
      delay(100);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(100);
    }
  }
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();

  if((unsigned long)(millis() - lastLedUpdate) >= LED_UPDATE_INTERVAL) {
    // gradientUpdate(strip1, 0x00FFFFFF, 0x0055FFFF);
    // gradientUpdate(strip2, 0x00FFFFFF, 0x0055FFFF);
    // gradientUpdate(strip3, 0x00FFFFFF, 0x0055FFFF);
    // gradientUpdate(strip4, 0x00FFFFFF, 0x0055FFFF);

    gradientUpdate2(strip1, 0x00FFFFFF);
    gradientUpdate2(strip2, 0x00FFFFFF);
    gradientUpdate2(strip3, 0x00FFFFFF);
    gradientUpdate2(strip4, 0x00FFFFFF);
    lastLedUpdate = millis();
  }

  // gradientFade(strip1, 0x00FFFFFF, 0x0055FFFF, 1000); // Example: Red to Blue on the first strip
  // gradientFade(strip2, 0x00FFFFFF, 0x0055FFFF, 1000); // Example: Green to Purple on the second strip
  // gradientFade(strip3, 0x00FFFFFF, 0x0055FFFF, 1000);
  // gradientFade(strip4, 0x00FFFFFF, 0x0055FFFF, 1000);

  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("Sensor: BPM: ");
    Serial.println(myBPM);
    tone(PIN_SPEAKER, 50);
    delay(50);
    lastBeatDetected = millis();
  }

  if(pulseSensor.isInsideBeat() == false){
    noTone(PIN_SPEAKER);
  }
}

void gradientUpdate(Adafruit_NeoPixel& strip, uint32_t color1, uint32_t color2) {
  ledGradientStep += ledFadeDirection;
  if(ledGradientStep == 255 && ledFadeDirection == 1) {
    ledFadeDirection = -1;
  } else if(ledGradientStep == 0 && ledFadeDirection == -1) {
    ledFadeDirection = 1;
  }

  uint32_t color = fadeColor(color1, color2, ledGradientStep);
  strip.fill(color);
  strip.show();
}

void gradientUpdate2(Adafruit_NeoPixel& strip, uint32_t color1) {
  int fadeStep = map(millis() - lastBeatDetected, 0, BEAT_FADE_TIME, 0, 255);
  if(fadeStep > 255) {
    fadeStep = 255;
  }
  uint32_t color = fadeColor(color1, 0x00000000, fadeStep);
  strip.fill(color);
  strip.show();
}

// void gradientFade(Adafruit_NeoPixel& strip, uint32_t color1, uint32_t color2, int wait) {
//   for (int i = 0; i < 256; i++) {
//     for (int j = 0; j < strip.numPixels(); j++) {
//       uint32_t color = fadeColor(color1, color2, i);
//       strip.setPixelColor(j, color);
//     }
//     strip.show();
//     delay(wait / 512); // Adjust for both fade in and out
//   }
//   for (int i = 255; i >= 0; i--) {
//     for (int j = 0; j < strip.numPixels(); j++) {
//       uint32_t color = fadeColor(color1, color2, i);
//       strip.setPixelColor(j, color);
//     }
//     strip.show();
//     delay(wait / 512);
//   }
// }

uint32_t fadeColor(uint32_t color1, uint32_t color2, uint8_t step) {
  uint8_t r = ((color1 >> 16) & 0xFF) + (((color2 >> 16) & 0xFF) - ((color1 >> 16) & 0xFF)) * step / 255;
  uint8_t g = ((color1 >> 8) & 0xFF) + (((color2 >> 8) & 0xFF) - ((color1 >> 8) & 0xFF)) * step / 255;
  uint8_t b = (color1 & 0xFF) + ((color2 & 0xFF) - (color1 & 0xFF)) * step / 255;
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}