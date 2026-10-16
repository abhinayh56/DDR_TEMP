#define ML_1_PIN 4
#define ML_2_PIN 5
#define ML_PWM_PIN 6

int pwm_L = 0;

void setup() {
  Serial.begin(115200);

  pinMode(ML_1_PIN, OUTPUT);
  pinMode(ML_2_PIN, OUTPUT);
  pinMode(ML_PWM_PIN, OUTPUT);
}

void loop() {
  if (Serial.available()>0)
  {
    String s = Serial.readString();
    pwm_L = s.toInt();
  }

  pwm_L = constrain(pwm_L, -255, 255);

  //Serial.println(pwm_L);

  if (pwm_L>=0)                                                                                                                 
  {
    digitalWrite(ML_1_PIN, HIGH);
    digitalWrite(ML_2_PIN, LOW);
  }
  else
  {
    digitalWrite(ML_1_PIN, LOW);
    digitalWrite(ML_2_PIN, HIGH);
  }
  analogWrite(ML_PWM_PIN, abs(pwm_L));
}