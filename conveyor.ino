#include <Servo.h>

// Constants
const int colorS0Pin = 2;        // Color sensor S0 pin
const int colorS1Pin = 3;        // Color sensor S1 pin
const int colorOutPin = 4;       // Color sensor OUT pin
const int topDistanceEchoPin = 5;      // Echo pin for top distance sensor
const int topDistanceTrigPin = 6;      // Trigger pin for top distance sensor
const int side1DistanceEchoPin = 7;    // Echo pin for side 1 distance sensor
const int side1DistanceTrigPin = 8;    // Trigger pin for side 1 distance sensor
const int side2DistanceEchoPin = 9;    // Echo pin for side 2 distance sensor
const int side2DistanceTrigPin = 10;   // Trigger pin for side 2 distance sensor
const int colorServoPin = 11;    // Digital pin for color sorting servo motor
const int sizeServoPin = 12;     // Digital pin for size sorting servo motor

// Thresholds for color detection
const int redThreshold = 500;    // Adjust based on your color sensor readings
const int greenThreshold = 500;  // Adjust based on your color sensor readings
const int blueThreshold = 500;   // Adjust based on your color sensor readings

// Sorting positions for the servo motors
const int smallColorPosition = 30;   // Adjust based on the color sorting mechanism
const int mediumColorPosition = 90;  // Adjust based on the color sorting mechanism
const int largeColorPosition = 150;  // Adjust based on the color sorting mechanism
const int smallSizePosition = 30;    // Adjust based on the size sorting mechanism
const int mediumSizePosition = 90;   // Adjust based on the size sorting mechanism
const int largeSizePosition = 150;   // Adjust based on the size sorting mechanism

// Create servo motor objects
Servo colorServo;
Servo sizeServo;

void setup() {
  // Initialize color sensor pins
  pinMode(colorS0Pin, OUTPUT);
  pinMode(colorS1Pin, OUTPUT);
  pinMode(colorOutPin, INPUT);

  // Initialize distance sensor pins
  pinMode(topDistanceEchoPin, INPUT);
  pinMode(topDistanceTrigPin, OUTPUT);
  pinMode(side1DistanceEchoPin, INPUT);
  pinMode(side1DistanceTrigPin, OUTPUT);
  pinMode(side2DistanceEchoPin, INPUT);
  pinMode(side2DistanceTrigPin, OUTPUT);

  // Initialize servo motors
  colorServo.attach(colorServoPin);
  sizeServo.attach(sizeServoPin);

  // Set the sorting servo motors to initial positions
  colorServo.write(smallColorPosition);
  sizeServo.write(smallSizePosition);

  // Initialize serial communication if needed
  // Serial.begin(9600);
}

void loop() {
  // Read color and size sensor values
  int colorValue = getColor();
  int topDistanceValue = getDistance(topDistanceTrigPin, topDistanceEchoPin);
  int side1DistanceValue = getDistance(side1DistanceTrigPin, side1DistanceEchoPin);
  int side2DistanceValue = getDistance(side2DistanceTrigPin, side2DistanceEchoPin);

  // Determine object color based on color sensor readings
  String objectColor;
  if (colorValue < redThreshold) {
    objectColor = "Red";
  } else if (colorValue < greenThreshold) {
    objectColor = "Green";
  } else if (colorValue < blueThreshold) {
    objectColor = "Blue";
  } else {
    objectColor = "Unknown";
  }

  // Determine object size based on distance sensor readings
  String objectSize;
  if (topDistanceValue < 10 && side1DistanceValue < 10 && side2DistanceValue < 10) {
    objectSize = "Small";
  } else if (topDistanceValue < 20 && side1DistanceValue < 20 && side2DistanceValue < 20) {
    objectSize = "Medium";
  } else {
    objectSize = "Large";
  }

  // Perform sorting based on color and size
  if (objectColor == "Red" && objectSize == "Small") {
    colorServo.write(smallColorPosition);
    sizeServo.write(smallSizePosition);
    // Additional actions or output can be added here
  } else if (objectColor == "Green" && objectSize == "Medium") {
    colorServo.write(mediumColorPosition);
    sizeServo.write(mediumSizePosition);
    // Additional actions or output can be added here
  } else if (objectColor == "Blue" && objectSize == "Large") {
    colorServo.write(largeColorPosition);
    sizeServo.write(largeSizePosition);
    // Additional actions or output can be added here
  }

  // Print color and size information if needed
  // Serial.print("Object Color: ");
  // Serial.println(objectColor);
  // Serial.print("Object Size: ");
  // Serial.println(objectSize);

  // Delay between sorting operations
  delay(1000);
}

// Function to read color value from TCS3200 color sensor
int getColor() {
  digitalWrite(colorS0Pin, HIGH);
  digitalWrite(colorS1Pin, LOW);
  int colorValue = pulseIn(colorOutPin, LOW);
  return colorValue;
}

// Function to read distance value from HCSR04 sensor
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Calculate distance in centimeters
  return distance;
}
