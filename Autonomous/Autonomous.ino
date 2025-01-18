#include <Servo.h>
 #include <NewPing.h>
 // Define pin connections and constants
 const int motor1Pin1 = 4; // PWM capable pin
 const int motor1Pin2 = 5; // PWM capable pin
 const int motor2Pin1 = 6; // PWM capable pin
 const int motor2Pin2 = 7; // PWM capable pin
 const int trigPin = 8;
 const int echoPin = 9;
 const int servoPin = 11;
 const int IRsensorPin = 35; // New IR sensor pin
const int irSensor1Pin = 2; // Existing IR sensor pins
 const int irSensor2Pin = 3;
 const int ledPin = 25; // Built-in LED pin
 const int blueLEDPin = 24;
 const int newServoPin = 10; // New servo pin
 const int maxSpeed = 180; // Adjusted maximum speed (PWM value)
 const int turnSpeed = 90; // Turning speed (half of maxSpeed for
 controlled turn)
 const int criticalDistance = 20; // Distance threshold in cm
 // Create servo and ultrasonic objects
 Servo myservo;
 Servo newServo; // New servo object
 NewPing sonar(trigPin, echoPin, 200); // Set max distance to 200 cm
 const unsigned long detectionDelay2 = 100;
 void setup() {
 myservo.attach(servoPin); // Attach the servo to its control pin
 newServo.attach(newServoPin); // Attach the new servo to its control
 pin
 pinMode(motor1Pin1, OUTPUT);
 pinMode(motor1Pin2, OUTPUT);
 pinMode(motor2Pin1, OUTPUT);
 pinMode(motor2Pin2, OUTPUT);
 pinMode(IRsensorPin, INPUT); // Set IR sensor pin as input
 pinMode(ledPin, OUTPUT); // Set LED pin as output
 pinMode(blueLEDPin, OUTPUT);
 Serial.begin(9600);
 randomSeed(analogRead(0)); // Initialize random seed
 }
 void loop() {
 // Check IR sensor and control LED and new servo accordingly
 newServo.write(180);
 checkLineSensors();
 int irValue = digitalRead(IRsensorPin);
 if (irValue == LOW) {
 digitalWrite(ledPin, HIGH);
 // Turn on the LED
 newServo.write(35); // Move the new servo to 180 degrees
 delay(100);
 digitalWrite(ledPin,LOW);
 } else {
 digitalWrite(ledPin, LOW); // Turn off the LED
 }
 int closestDistance = 999; // Initialize to a high value
 int closestAngle = 0; // Angle at which the closest obstacle is
 detected
 int dangerCount = 0; // Count of measurements below the critical
 distance
 // Sweep servo with ultrasonic sensor from 0 to 180 degrees
 for (int pos = 0; pos <= 180; pos += 15) {
 myservo.write(pos);
 delay(100); // Increased delay for stable readings
 int distance = sonar.ping_cm();
 Serial.print("Angle: ");
 Serial.print(pos);
Serial.print("°, Distance: ");
 Serial.println(distance);
 // Update closest distance and angle if a closer obstacle is detected
 if (distance > 0 && distance < closestDistance) {
 closestDistance = distance;
 closestAngle = pos;
 }
 // Increment danger count if distance is below the critical threshold
 if (distance > 0 && distance < criticalDistance) {
 dangerCount++;
 }
 }
 // React based on the closest obstacle detected
 if (dangerCount > 3) { // If more than 3 measurements are below the
 critical distance
 stopMotors(); // Stop the motors
 delay(6000); // Short pause after stopping
 moveBackward(); // Move backward for 3 seconds
 delay(3000);
 stopMotors(); // Stop moving backward
 delay(500); // Short pause after moving backward
 randomTurn(); // Take a random turn
 } else if (closestDistance < 50 && closestDistance > 0) {
 if (closestAngle < 90) {
 turnRight();
 } else if (closestAngle > 90) {
 turnLeft();
 } else {
 moveForward();
 }
 } else {
 moveForward();
 }
 }
 void checkLineSensors() {
 int irSensor1Value = digitalRead(irSensor1Pin);
 int irSensor2Value = digitalRead(irSensor2Pin);
 // Check if a large width white line is detected
 if (irSensor1Value == LOW && irSensor2Value == LOW) {
 delay(detectionDelay2);
 irSensor1Value = digitalRead(irSensor1Pin);
 irSensor2Value = digitalRead(irSensor2Pin);
 // Check if only one sensor detects a white line
 if (irSensor1Value == LOW && irSensor2Value == LOW) {
 // Blink blue LED
 blinkBlueLED();
 stopMotors(); // Stop the motors
 delay(600); // Short pause after stopping
 moveBackward(); // Move backward for 3 seconds
 delay(3000);
 stopMotors(); // Stop moving backward
 delay(500); // Short pause after moving backward
 randomTurn();
 }
}
 }
 void blinkBlueLED() {
 digitalWrite(blueLEDPin, HIGH);
 delay(500);
 digitalWrite(blueLEDPin, LOW);
 delay(500);
 }
 void moveForward() {
 analogWrite(motor1Pin1, maxSpeed);
 digitalWrite(motor1Pin2, LOW);
 analogWrite(motor2Pin1, maxSpeed);
 digitalWrite(motor2Pin2, LOW);
 Serial.println("Moving Forward");
 }
 void turnLeft() {
 digitalWrite(motor1Pin1, LOW);
 digitalWrite(motor1Pin2, HIGH);
 digitalWrite(motor2Pin1, HIGH);
 digitalWrite(motor2Pin2, LOW);
 Serial.println("Turning Left");
 }
 void turnRight() {
 digitalWrite(motor1Pin1, HIGH);
 digitalWrite(motor1Pin2, LOW);
 digitalWrite(motor2Pin1, LOW);
 digitalWrite(motor2Pin2, HIGH);
 Serial.println("Turning Right");
 }
 void moveBackward() {
 analogWrite(motor1Pin1, LOW);
 digitalWrite(motor1Pin2, HIGH);
 analogWrite(motor2Pin1, LOW);
 digitalWrite(motor2Pin2, HIGH);
 Serial.println("Moving Backward");
 }
 void stopMotors() {
 analogWrite(motor1Pin1, LOW);
 digitalWrite(motor1Pin2, LOW);
 analogWrite(motor2Pin1, LOW);
 digitalWrite(motor2Pin2, LOW);
 Serial.println("Stopping Motors");
 }
 void randomTurn() {
 int turnDirection = random(0, 2); // Generate a random number: 0 or 1
 if (turnDirection == 0) {
 turnLeft();
 } else {
 turnRight();
 }
 delay(1000); // Turn for 1 second
 stopMotors();
 }
