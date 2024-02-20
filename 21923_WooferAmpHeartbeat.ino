#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;

const int PULSE_INPUT = A0;
const int PULSE_BLINK = LED_BUILTIN;
const int PULSE_FADE = 5;
const int THRESHOLD = 400;

const int PIN_SPEAKER = 9;

PulseSensorPlayground pulseSensor;

void setup() {
  Serial.begin(115200);


  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(THRESHOLD);

  pinMode(PIN_SPEAKER, OUTPUT);

  if (!pulseSensor.begin()) {
    for(;;) {
      digitalWrite(PULSE_BLINK, LOW);
      delay(100);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(100);
    }
  }
}

void loop() {
  if(pulseSensor.UsingHardwareTimer){
    delay(20);
    pulseSensor.outputSample();
  } else {
    if (pulseSensor.sawNewSample()) {
      if (--pulseSensor.samplesUntilReport == (byte) 0) {
        pulseSensor.samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
        pulseSensor.outputSample();
      }
    }
  }

  if (pulseSensor.sawStartOfBeat()) {
    pulseSensor.outputBeat();
    tone(PIN_SPEAKER, 165);  // tone(pin, frequency)
    delay(50);  // Turn on vibration motor
  }

  if(pulseSensor.isInsideBeat() == false){
    noTone(PIN_SPEAKER);
    delay(50);
  }
}