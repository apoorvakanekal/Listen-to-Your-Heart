#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;

// Define pins for both Pulse Sensors
const int PULSE_INPUT0 = A0;
const int PULSE_INPUT1 = A5;

const int PULSE_BLINK0 = 8;  // Assuming you want to blink the built-in LED for one sensor
const int PULSE_BLINK1 = 12;  // Additional LED for the second sensor, choose an appropriate pin
const int PULSE_MATCH = 11;  // LED that indicates BPM match

const int THRESHOLD0 = 600; // Threshold for first sensor
const int THRESHOLD1 = 600; // Threshold for second sensor

const int PULSE_SENSOR_COUNT = 2;  // Number of Pulse Sensors

PulseSensorPlayground pulseSensor(PULSE_SENSOR_COUNT);

const int PIN_SPEAKER = 3; // First speaker
const int PIN_SPEAKER2 = 6; // Second speaker, for average BPM

void setup() {
  Serial.begin(115200);

  // Setup for the first Pulse Sensor
  pulseSensor.analogInput(PULSE_INPUT0, 0);
  pulseSensor.blinkOnPulse(PULSE_BLINK0, 0);
  pulseSensor.setThreshold(THRESHOLD0, 0);

  // Setup for the second Pulse Sensor
  pulseSensor.analogInput(PULSE_INPUT1, 1);
  pulseSensor.blinkOnPulse(PULSE_BLINK1, 1);
  pulseSensor.setThreshold(THRESHOLD1, 1);

  // Setup for speakers and match LED
  pinMode(PULSE_MATCH, OUTPUT);
  pinMode(PIN_SPEAKER, OUTPUT);
  pinMode(PIN_SPEAKER2, OUTPUT);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);

  if (!pulseSensor.begin()) {
    Serial.println("Failed to initialize Pulse Sensor");
    for(;;) {
      digitalWrite(PULSE_BLINK0, LOW);
      delay(100);
      digitalWrite(PULSE_BLINK0, HIGH);
      delay(100);
    }
  }
}

void loop() {
    int myBPM1 = pulseSensor.getBeatsPerMinute(0);
    int myBPM2 = pulseSensor.getBeatsPerMinute(1);

    if (pulseSensor.sawStartOfBeat(0) && myBPM1 > 0) {
        // Serial.print("Sensor 1: BPM: ");
        // Serial.println(myBPM1);
        tone(PIN_SPEAKER, 50, 100); // Simulate heartbeat with a quick tone
    }
    
    if (pulseSensor.sawStartOfBeat(1) && myBPM2 > 0) {
        // Serial.print("Sensor 2: BPM: ");
        // Serial.println(myBPM2);
    }

    if (myBPM1 > 0 && myBPM2 > 0) {
    int avgBPM = (myBPM1 + myBPM2) / 2;
    Serial.print("Average BPM: ");
    Serial.println(avgBPM);

    // Check if average BPM is within a reasonable range
    if (avgBPM >= 40 && avgBPM <= 180) {
        unsigned long beepInterval = 60000 / avgBPM; // Calculate interval between beeps
        static unsigned long lastBeepTime = 0;
        if (millis() - lastBeepTime >= beepInterval) {
            tone(PIN_SPEAKER2, 50, 100); // Beep
            lastBeepTime = millis();
        }
    }
}

    // Logic to blink the matched LED if BPMs are close enough
    // if (myBPM1 > 0 && myBPM2 > 0 && abs(myBPM1 - myBPM2) <= 2) {
    //     digitalWrite(PULSE_MATCH, HIGH);
    //     delay(100); // Keep the LED on for 100 milliseconds
    //     digitalWrite(PULSE_MATCH, LOW);
    // } else {
    //     digitalWrite(PULSE_MATCH, LOW);
    // }
}