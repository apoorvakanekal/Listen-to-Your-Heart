#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;

// Define pins for both Pulse Sensors
const int PULSE_INPUT0 = A0;
const int PULSE_INPUT1 = A1;

const int PULSE_BLINK0 = LED_BUILTIN;  // Assuming you want to blink the built-in LED for one sensor
const int PULSE_BLINK1 = 11;            // Additional LED for the second sensor, choose an appropriate pin

const int PULSE_FADE0 = 5;  // PWM pin for first sensor
const int PULSE_FADE1 = 6;  // PWM pin for second sensor, make sure it supports PWM

const int THRESHOLD0 = 550; // Threshold for first sensor
const int THRESHOLD1 = 580; // Threshold for second sensor

const int PIN_SPEAKER = 9;  // Speaker pin

const int PULSE_SENSOR_COUNT = 2;  // Number of Pulse Sensors

PulseSensorPlayground pulseSensor(PULSE_SENSOR_COUNT);

void setup() {
  Serial.begin(115200);

  // Setup for the first Pulse Sensor
  pulseSensor.analogInput(PULSE_INPUT0, 0);
  pulseSensor.blinkOnPulse(PULSE_BLINK0, 0);
  pulseSensor.fadeOnPulse(PULSE_FADE0, 0);
  pulseSensor.setThreshold(THRESHOLD0, 0);

  // Setup for the second Pulse Sensor
  pulseSensor.analogInput(PULSE_INPUT1, 1);
  pulseSensor.blinkOnPulse(PULSE_BLINK1, 1);
  pulseSensor.fadeOnPulse(PULSE_FADE1, 1);
  pulseSensor.setThreshold(THRESHOLD1, 1);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);

  pinMode(PIN_SPEAKER, OUTPUT);

  if (!pulseSensor.begin()) {
    Serial.println("Failed to initialize Pulse Sensor");
    for(;;) {  // Infinite loop
      digitalWrite(PULSE_BLINK0, LOW);
      delay(100);
      digitalWrite(PULSE_BLINK0, HIGH);
      delay(100);
    }
  }
}

void loop() {
  if (pulseSensor.sawNewSample()) {
    // Assuming you don't need to change the way samples are output
  }

  for (int i = 0; i < PULSE_SENSOR_COUNT; i++) {
    if (pulseSensor.sawStartOfBeat(i)) {
      int myBPM = pulseSensor.getBeatsPerMinute(i);  // Get BPM for current sensor
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(" BPM: ");
      Serial.println(myBPM);
      
      if (i == 0) {  // Optionally use the speaker for the first sensor only
        tone(PIN_SPEAKER, 165);
        delay(50);
        noTone(PIN_SPEAKER);
      }
    }
  }
}
