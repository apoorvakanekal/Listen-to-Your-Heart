#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;

const int PULSE_INPUT = A0;
const int PULSE_BLINK = LED_BUILTIN;
const int PULSE_FADE = 5;
const int THRESHOLD = 400;

const int PIN_SPEAKER = 2;
const int PIN_VIBRATION_MOTOR = 9;  // Connect the vibration motor to digital pin 9

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
  pinMode(PIN_VIBRATION_MOTOR, OUTPUT);

  if (!pulseSensor.begin()) {
    for(;;) {
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
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
    tone(PIN_SPEAKER, 100, 500);  // tone(pin, frequency)
    digitalWrite(PIN_VIBRATION_MOTOR, HIGH);
    delay(50);  // Turn on vibration motor
  }

  if(pulseSensor.isInsideBeat() == false){
    noTone(PIN_SPEAKER);
    digitalWrite(PIN_VIBRATION_MOTOR, LOW); // Turn off vibration motor
    delay(50);
  }
}
