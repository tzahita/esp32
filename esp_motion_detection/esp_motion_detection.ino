#include "HomeSpan.h"

const int pirPin = 13;
bool motionDetectionEnabled = true;  // Start with motion detection enabled

// Declare MotionDetection globally
struct MotionDetection : Service::MotionSensor {  // Use the MotionSensor service for motion detection

  int pirPin;                  // This variable stores the pin number for the PIR sensor
  SpanCharacteristic* motion;  // Pointer to a SpanCharacteristic to represent motion detected

  // Constructor for MotionDetection, taking the pin number of the PIR sensor
  MotionDetection(int pirPin)
    : Service::MotionSensor() {

    motion = new Characteristic::MotionDetected();  // Create the MotionDetected characteristic
    this->pirPin = pirPin;                          // Store the PIR sensor pin number
    pinMode(pirPin, INPUT);                         // Set the PIR sensor pin to INPUT
  }

  // Override the update method to handle motion detection logic
  boolean update() override {
    if (motionDetectionEnabled) {
      bool motionDetected = digitalRead(pirPin) == HIGH;
      motion->setVal(motionDetected);  // Update the motion characteristic with the new value
      if (motionDetected) {
        Serial.println("Motion detected!");
      } else {
        Serial.println("No motion.");
      }
    } else {
      motion->setVal(false);  // Ensure no motion is detected if motion detection is disabled
    }
    return true;  // Return true to indicate the update was successful
  }
};

// Declare a global pointer for MotionDetection service
MotionDetection* motionDetectionService;

struct MotionDetectionSwitch : Service::Switch {
  SpanCharacteristic* power;

  MotionDetectionSwitch()
    : Service::Switch() {
    power = new Characteristic::On(motionDetectionEnabled);
  }

  boolean update() override {
    motionDetectionEnabled = power->getNewVal();  // Enable or disable motion detection
    Serial.println(motionDetectionEnabled ? "Motion detection enabled" : "Motion detection disabled");
    return true;  // Return true to indicate success
  }
};

void setup() {
  Serial.begin(115200);
  homeSpan.setPairingCode("11122333");

  homeSpan.begin(Category::SecuritySystems, "Motion Detection");

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();

  motionDetectionService = new MotionDetection(pirPin);  // Create and store the motion detection service
  new MotionDetectionSwitch();                           // Create the switch service
}

void loop() {
  homeSpan.poll();  // Handle HomeKit communication

  // Call the update method of MotionDetection periodically
  if (motionDetectionService) {
    motionDetectionService->update();
  }

  delay(200);  // Short delay to avoid bouncing
}
