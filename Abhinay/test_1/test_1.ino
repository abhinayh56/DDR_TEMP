// Motor A connections
int pwmA = 4;
int Ain1 = 6;
int Ain2 = 5;

void setup() {
  // Set all the motor control pins to outputs
  pinMode(pwmA, OUTPUT);

  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, LOW);
}

void loop() {
  // directionControl();
  delay(1000);
  speedControl();
  delay(1000);
}

// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  digitalWrite(pwmA, HIGH);

  // Turn on motor A & B
  digitalWrite(Ain1, HIGH);
  digitalWrite(Ain2, LOW);
  delay(2000);

  // Now change motor directions
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);
  delay(2000);

  // Turn off motors
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors and run ccw
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, HIGH);

  // Accelerate from zero to maximum speed
  for (int i = 0; i < 180; i++) {
    analogWrite(pwmA, i);
    delay(50);
  }

  // Decelerate from maximum speed to zero
  for (int i = 180; i >= 0; --i) {
    analogWrite(pwmA, i);
    delay(50);
  }

  // Now turn off motors
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, LOW);

  // Turn on motors and run cw
  digitalWrite(Ain1, HIGH);
  digitalWrite(Ain2, LOW);

  // Accelerate from zero to maximum speed
  for (int i = 0; i < 180; i++) {
    analogWrite(pwmA, i);
    delay(50);
  }

  // Decelerate from maximum speed to zero
  for (int i = 180; i >= 0; --i) {
    analogWrite(pwmA, i);
    delay(50);
  }

  // Now turn off motors
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, LOW);
}
