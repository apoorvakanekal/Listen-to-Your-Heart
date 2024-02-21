#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.

// Variables for sensor 1
const int PulseWire1 = A0;     // PulseSensor 1 PURPLE WIRE connected to ANALOG PIN A0
const int LED1 = LED_BUILTIN;  // The on-board Arduino LED, close to PIN 13.
int Threshold1 = 480;          // Determine which Signal to "count as a beat" for sensor 1

// Variables for sensor 2
const int PulseWire2 = A1;     // PulseSensor 2 PURPLE WIRE connected to ANALOG PIN A1
int Threshold2 = 480;          // Determine which Signal to "count as a beat" for sensor 2
// Note: Using LED1 for both since most Arduinos have only one built-in LED. 
// Consider using an external LED if you want to visualize beats for both sensors.

// Create instances of the PulseSensorPlayground object for both sensors
PulseSensorPlayground pulseSensor1;
PulseSensorPlayground pulseSensor2;

void setup() {
  Serial.begin(9600);  // For Serial Monitor

  // Configure the PulseSensor objects, by assigning our variables to them.
  pulseSensor1.analogInput(PulseWire1);
  pulseSensor1.blinkOnPulse(LED1);  // Optionally blink LED with heartbeat of sensor 1.
  pulseSensor1.setThreshold(Threshold1);

  pulseSensor2.analogInput(PulseWire2);
  // pulseSensor2.blinkOnPulse(LED2);  // If you have a second LED, you can enable this
  pulseSensor2.setThreshold(Threshold2);

  // Check if the pulseSensor objects were created and began seeing a signal.
  if (pulseSensor1.begin()) {
    Serial.println("Sensor 1 ready!");
  }
  if (pulseSensor2.begin()) {
    Serial.println("Sensor 2 ready!");
  }
}

void loop() {
  if (pulseSensor1.sawStartOfBeat()) {
    int myBPM1 = pulseSensor1.getBeatsPerMinute();
    Serial.print("Sensor 1 BPM: ");
    Serial.println(myBPM1);
  }

  if (pulseSensor2.sawStartOfBeat()) {
    int myBPM2 = pulseSensor2.getBeatsPerMinute();
    Serial.print("Sensor 2 BPM: ");
    Serial.println(myBPM2);
  }

  delay(20);  // Small delay to keep things responsive but not too fast.
}
