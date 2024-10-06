#include <Wire.h>

// Motor control pins
const int motorPin1 = 4;  // PWM pin connected to motor controller for axis_4
const int motorPin2 = 3;  // PWM pin connected to motor controller for axis_5



// Direction control pins
const int dirPin1 = 5;   // Direction control pin for motor 1
const int dirPin2 = 6;   // Direction control pin for motor 2
const int dirPin3 = 7;   // Direction control pin for motor 3
const int dirPin4 = 8;   // Direction control pin for motor 4



int buttonState = 0;      // Variable to store the state of Button 0

// Single Brake control pin
const int brakePin = 9;  // Single brake pin for the motor

int currentSpeed1 = 0;    // Variable to store the current motor speed for axis_4
int currentSpeed2 = 0;    // Variable to store the current motor speed for axis_5

void setup() {
  Serial.begin(9600);   // Start serial communication at 9600 baud rate
  Wire.begin();         // Initialize I2C communication (if needed)
  
  pinMode(motorPin1, OUTPUT);  // Set motorPin1 as output
  pinMode(motorPin2, OUTPUT);  // Set motorPin2 as output

  Serial.println("Waiting for axis 4 and axis 5 input...");




  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(dirPin4, OUTPUT);
  pinMode(brakePin, OUTPUT);   // Set brakePin as output

  // Initialize direction to default (e.g., all LOW)
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, LOW);
  digitalWrite(dirPin3, LOW);
  digitalWrite(dirPin4, HIGH);

   // Initialize brake to on
  digitalWrite(brakePin, HIGH);
}

void loop() {
  if (Serial.available() > 0) {      // Check if there is serial input
    // Read the incoming data as a string
    String data = Serial.readStringUntil('\n');

    // Parse the string to extract the axis 4 and axis 5 values
    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.lastIndexOf(',');
    

    if (commaIndex1 > 0 && commaIndex2 > commaIndex1) {
      int axis_4 = data.substring(0, commaIndex1).toInt();  // Extract axis 4 value
      int axis_5 = data.substring(commaIndex1 + 1, commaIndex2).toInt();  // Extract axis 5 value
      buttonState = data.substring(commaIndex2 + 1).toInt();  // Extract Button 0 state (0 or 1)

      // Map the axis 4 value (already 0-255) directly to PWM for motor 1
      int pwmValue1 = constrain(axis_4, 0, 255);

      // Map the axis 5 value (already 0-255) directly to PWM for motor 2
      int pwmValue2 = constrain(axis_5, 0, 255);

      // Update motor 1 speed if the new speed is different
      if (pwmValue1 != currentSpeed1) {
        currentSpeed1 = pwmValue1;            // Update the current speed
        analogWrite(motorPin1, currentSpeed1);  // Set the motor speed for axis_4

        // Print the set speed to the serial monitor for axis_4
        Serial.print("Motor 1 speed set to: ");
        Serial.print(map(currentSpeed1, 0, 255, 0, 100));  // Show equivalent speed in 0-100 range
        Serial.print(" (PWM value: ");
        Serial.print(currentSpeed1);
        Serial.println(")");
      }

      // Update motor 2 speed if the new speed is different
      if (pwmValue2 != currentSpeed2) {
        currentSpeed2 = pwmValue2;            // Update the current speed
        analogWrite(motorPin2, currentSpeed2);  // Set the motor speed for axis_5

        // Print the set speed to the serial monitor for axis_5
        Serial.print("Motor 2 speed set to: ");
        Serial.print(map(currentSpeed2, 0, 255, 0, 100));  // Show equivalent speed in 0-100 range
        Serial.print(" (PWM value: ");
        Serial.print(currentSpeed2);
        Serial.println(")");
      }


      if (currentSpeed1 == 0 || currentSpeed2 == 0) {
        digitalWrite(brakePin, HIGH);  // Engage brake
      } else {
        digitalWrite(brakePin, LOW);   // Release brake
      }


      // Perform additional logic based on the Button 0 state
      if (buttonState == 1) {
          digitalWrite(dirPin1, HIGH);
          digitalWrite(dirPin2, LOW);
          digitalWrite(dirPin3, LOW);
          digitalWrite(dirPin4, HIGH);
      } else {
          digitalWrite(dirPin1, LOW);
          digitalWrite(dirPin2, HIGH);
          digitalWrite(dirPin3, HIGH);
          digitalWrite(dirPin4, LOW);
      }
    }
  }
  
  // The motors will continue running at the last set speed
}
