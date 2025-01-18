 #include <nRF24L01.h>
 #include <RF24.h>
 #include <Servo.h>
 RF24 radio(13, 12); // CE, CSN pins
 // Define motor pins
 const int motor1Pin1 = 4;
 const int motor1Pin2 = 5;
 const int motor2Pin1 = 6;
 const int motor2Pin2 = 7;
 // IR Sensors
 const int irSensor1Pin = 2; // Existing IR sensor pins
 const int irSensor2Pin = 3;
 const int irSensor3Pin = 35; // Additional IR sensor for servo trigger
 // LED Pins
 const int redLEDPin = 22;
 const int greenLEDPin = 23;
 const int blueLEDPin = 24;
 const int yellowLEDPin = 25; // New Yellow LED pin
 // Servo
 Servo servo;
 const int servoPin = 10;
 const unsigned long detectionDelay = 400;
 const unsigned long detectionDelay2 = 100;
 void setup() {
 pinMode(motor1Pin1, OUTPUT);
 pinMode(motor1Pin2, OUTPUT);
 pinMode(motor2Pin1, OUTPUT);
 pinMode(motor2Pin2, OUTPUT);
 pinMode(irSensor1Pin, INPUT);
 pinMode(irSensor2Pin, INPUT);
 pinMode(irSensor3Pin, INPUT);
 // Initialize LED pins as outputs
 pinMode(greenLEDPin, OUTPUT);
 pinMode(redLEDPin, OUTPUT);
 pinMode(blueLEDPin, OUTPUT);
 pinMode(yellowLEDPin, OUTPUT);
 // Initialize and attach servo
 servo.attach(servoPin);
 // Start servo at 0 degrees
Serial.begin(9600);
 radio.begin();
 radio.setPALevel(RF24_PA_LOW);
 const byte address[6] = "11010"; // Address for communication
 radio.openReadingPipe(1, address);
 radio.startListening();
 }
 void loop() {
 servo.write(180);
 checkLineSensors();
 checkServoTriggerSensor();
 receiveCommands();
 }
 void checkLineSensors() {
 int irSensor1Value = digitalRead(irSensor1Pin);
 int irSensor2Value = digitalRead(irSensor2Pin);
 // Check if both sensors detect a white line
 if (irSensor1Value == LOW && irSensor2Value == LOW) {
 // Stop the vehicle
 // Wait for a short period to check for another white line
 delay(detectionDelay);
 // Read values from IR sensors again
 irSensor1Value = digitalRead(irSensor1Pin);
 irSensor2Value = digitalRead(irSensor2Pin);
 // Check if only one sensor detects a white line
 if (irSensor1Value == HIGH && irSensor2Value == HIGH) {
 // Blink green LED
 blinkRedLED();
 } else if (irSensor1Value == LOW && irSensor2Value == LOW) {
 delay(detectionDelay);
 irSensor1Value = digitalRead(irSensor1Pin);
 irSensor2Value = digitalRead(irSensor2Pin);
 if (irSensor1Value == HIGH && irSensor2Value == HIGH) {
 // Blink red LED
 blinkGreenLED();
 }
 }
 }
 // Check if a large width white line is detected
 if (irSensor1Value == LOW && irSensor2Value == LOW) {
 delay(detectionDelay2);
 irSensor1Value = digitalRead(irSensor1Pin);
 irSensor2Value = digitalRead(irSensor2Pin);
 // Check if only one sensor detects a white line
 if (irSensor1Value == LOW && irSensor2Value == LOW) {
 // Blink blue LED
 blinkBlueLED();
 }
 }
 }
void checkServoTriggerSensor() {
 static int c = 0; // Make c static so it retains its value across
 function calls
 int irSensor3Value = digitalRead(irSensor3Pin);
 if (irSensor3Value == LOW) {
 c++;
 digitalWrite(yellowLEDPin, HIGH);
 delay(100);
 digitalWrite(yellowLEDPin, LOW);
 Serial.println(c);
 }
 if(c == 20)
 {
 servo.write(35);
 delay(3000);
 servo.write(180);
 c = 0; // Reset c to 0 after completing the operation
 }
 }
 void receiveCommands() {
 if (radio.available()) {
 int command;
 radio.read(&command, sizeof(command));
 // Perform actions based on received command
 switch(command) {
 case 1: // Forward
 moveForward();
 Serial.println("Moving Forward");
 break;
 case 2: // Right
 turnRight();
 Serial.println("Turning Right");
 break;
 case 3: // Left
 turnLeft();
 Serial.println("Turning Left");
 break;
 case 4: // Backward
 moveBackward();
 Serial.println("Moving Backward");
 break;
 case 5: // Backward
 stop();
 Serial.println("s");
 break;
 }
 }
 }
 // Function to blink green LED
 void blinkGreenLED() {
 digitalWrite(greenLEDPin, HIGH);
 delay(500);
 digitalWrite(greenLEDPin, LOW);
 delay(500);
 }
 // Function to blink red LED
void blinkRedLED() {
 digitalWrite(redLEDPin, HIGH);
 delay(500);
 digitalWrite(redLEDPin, LOW);
 delay(500);
 }
 // Function to blink blue LED
 void blinkBlueLED() {
 digitalWrite(blueLEDPin, HIGH);
 delay(500);
 digitalWrite(blueLEDPin, LOW);
 delay(500);
 }
 // FUnction to move forward
 void moveForward() {
 digitalWrite(motor1Pin1, HIGH);
 digitalWrite(motor1Pin2, LOW);
 digitalWrite(motor2Pin1, HIGH);
 digitalWrite(motor2Pin2, LOW);
 }
 //Function to move Backward
 void moveBackward() {
 digitalWrite(motor1Pin1, LOW);
 digitalWrite(motor1Pin2, HIGH);
 digitalWrite(motor2Pin1, LOW);
 digitalWrite(motor2Pin2, HIGH);
 }
 //Function to move Left
 void turnLeft() {
 digitalWrite(motor1Pin1, LOW);
 digitalWrite(motor1Pin2, HIGH);
 digitalWrite(motor2Pin1, HIGH);
 digitalWrite(motor2Pin2, LOW);
 }
 // Function to move Right
 void turnRight() {
 digitalWrite(motor1Pin1, HIGH);
 digitalWrite(motor1Pin2, LOW);
 digitalWrite(motor2Pin1, LOW);
 digitalWrite(motor2Pin2, HIGH);
 }
 // Function to Stop
 void stop(){
 digitalWrite(motor1Pin1, LOW);
 digitalWrite(motor1Pin2, LOW);
 digitalWrite(motor2Pin1, LOW);
 digitalWrite(motor2Pin2, LOW);
 }