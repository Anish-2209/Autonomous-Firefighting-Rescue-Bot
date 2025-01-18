#include <SPI.h>
#include <nRF24L01.h>
 #include <RF24.h>
 RF24 radio(9, 10); // CE, CSN
 const int joystickXPin = A0;
 const int joystickYPin = A1;
 int joystickXValue;
 int joystickYValue;
 void setup() {
 Serial.begin(9600);
 radio.begin();
 radio.setPALevel(RF24_PA_LOW);
 const byte address[6] = "11010"; // Address for communication
 radio.openWritingPipe(address);
 }
 void loop() {
 joystickXValue = analogRead(joystickXPin);
 joystickYValue = analogRead(joystickYPin);
 int command = 5;
 // Map joystick values to commands
 if (joystickYValue < 400) {
 command = 1; // Forward
 } else if (joystickYValue > 600) {
 command = 4; // Backward
 } else if (joystickXValue < 400) {
 command = 3; // Left
 } else if (joystickXValue > 600) {
 command = 2; // Right
 }
 Serial.println(command);
 radio.write(&command, sizeof(command));
 }