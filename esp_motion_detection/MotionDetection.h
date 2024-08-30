struct MotionDetection : Service::MotionSensor {  // Use the MotionSensor service for motion detection

  int pirPin;                  // this variable stores the pin number for the PIR sensor
  SpanCharacteristic *motion;  // pointer to a SpanCharacteristic to represent motion detected

  // Constructor for MotionDetection, taking the pin number of the PIR sensor
  MotionDetection(int pirPin)
    : Service::MotionSensor() {

    motion = new Characteristic::MotionDetected();  // Create the MotionDetected characteristic
    this->pirPin = pirPin;                          // Store the PIR sensor pin number
    pinMode(pirPin, INPUT);                         // Set the PIR sensor pin to INPUT
  }

  // No need to override the update method in this case
};