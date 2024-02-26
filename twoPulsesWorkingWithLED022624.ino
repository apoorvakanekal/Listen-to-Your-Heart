#include <PulseSensorPlayground.h>

const int OUTPUT_TYPE = SERIAL_PLOTTER;

// Define pins for both Pulse Sensors
const int PULSE_INPUT0 = A0;
const int PULSE_INPUT1 = A5;

const int PULSE_BLINK0 = 10;  // Assuming you want to blink the built-in LED for one sensor
const int PULSE_BLINK1 = 11;            // Additional LED for the second sensor, choose an appropriate pin
const int PULSE_MATCH = 3;

const int THRESHOLD0 = 560; // Threshold for first sensor
const int THRESHOLD1 = 560; // Threshold for second sensor

const int PULSE_SENSOR_COUNT = 2;  // Number of Pulse Sensors

PulseSensorPlayground pulseSensor(PULSE_SENSOR_COUNT);

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

  //setup matched LED
  pinMode(PULSE_MATCH, OUTPUT);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);


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

 int myBPM1 = pulseSensor.getBeatsPerMinute(0); 
 int myBPM2 = pulseSensor.getBeatsPerMinute(1); 

  if (pulseSensor.sawStartOfBeat(0)) {            // Constantly test to see if "a beat happened".
    Serial.print("Sensor 1: BPM: "); // If test is "true", print a message "a heartbeat happened".                       // Print phrase "BPM: " 
    Serial.println(myBPM1);                        // Print the value inside of myBPM. 
}
  if (pulseSensor.sawStartOfBeat(1)) {            // Constantly test to see if "a beat happened". 
    Serial.print("Sensor 2: BPM: "); // If test is "true", print a message "a heartbeat happened".                       // Print phrase "BPM: " 
    Serial.println(myBPM2);                        // Print the value inside of myBPM. 
}
  if (myBPM1 == myBPM2){
    digitalWrite(PULSE_MATCH, HIGH);
}
  else{
    digitalWrite(PULSE_MATCH, LOW);
}
}
