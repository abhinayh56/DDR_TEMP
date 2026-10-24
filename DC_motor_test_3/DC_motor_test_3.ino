#define ENCL_A_PIN 2
#define ENCL_B_PIN 3
#define ML_1_PIN 4
#define ML_2_PIN 5
#define ML_PWM_PIN 6

int count_actual = 0;
int pwm_L = 0;

int count_target = 1000;
float kp = 0.52; // 0.5
float ki = 0.00625; // 0.3  0.00625
float kd = 0.03; // 0.03
float u = 0.0;

int e = 0;
int e_prev = 0;
uint64_t t_curr = 0;
uint64_t t_prev = 0;
float dt = 0.0;
float e_integral = 0.0;

volatile int pos = 0;
volatile int a_curr = false;
volatile int a_prev = false;
volatile int b_curr = false;
volatile int b_prev = false;

void setup() {
  Serial.begin(115200);
  pinMode(ENCL_A_PIN, INPUT);
  pinMode(ENCL_B_PIN, INPUT);
  pinMode(ML_1_PIN, OUTPUT);
  pinMode(ML_2_PIN, OUTPUT);
  pinMode(ML_PWM_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENCL_A_PIN), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCL_B_PIN), readEncoder, CHANGE);

  delay(1000);
}

void loop() {
  noInterrupts();
  count_actual = pos;
  interrupts();

  e = count_target - count_actual;

  t_curr = micros();
  dt = (float)(t_curr - t_prev)/1.0e6;
  t_prev = t_curr;

  if(abs(e) <= 725)
  {
    e_integral = e_integral + ki*e*dt;
    e_integral = constrain(e_integral, -150, 150);
  }
  else
  {
    e_integral = 0;
  }

  u = (int)(kp*e + e_integral + kd*(e - e_prev)/dt);
  u = constrain(u, -200, 200);

  if (u<0)
  {
    digitalWrite(ML_1_PIN, HIGH);
    digitalWrite(ML_2_PIN, LOW);
  }
  else
  {
    digitalWrite(ML_1_PIN, LOW);
    digitalWrite(ML_2_PIN, HIGH);
  }
  analogWrite(ML_PWM_PIN, abs(u));

  Serial.print(0);
  Serial.print(",");
  Serial.print(count_target);
  Serial.print(",");
  Serial.print(count_actual);
  Serial.print(",");
  // Serial.print(e);
  // Serial.print(",");
  Serial.println(u);

  e_prev = e;
  delay(5);

}

void readEncoder()
{
  a_curr = digitalRead(ENCL_A_PIN);
  b_curr = digitalRead(ENCL_B_PIN);

  if(a_curr != a_prev)
  {
    if (a_curr != b_curr)
    {
      pos++;
    }
    else
    {
      pos--;
    }
  }
  else
  {
    if (a_curr == b_curr)
    {
      pos++;
    }
    else
    {
      pos--;
    }
  }

  a_prev = a_curr;
  b_prev = b_curr;
}