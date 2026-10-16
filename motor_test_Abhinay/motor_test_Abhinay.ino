int ML_1_PIN = 4;
int ML_2_PIN = 5;
int ML_PWM_PIN = 6;

int ENCL_A_PIN = 2;
int ENCL_B_PIN = 3;

int ENC_CPR = 5800;

int pwm_L = 0;

volatile bool encL_A_prev = false;
volatile bool encL_B_prev = false;
volatile bool encL_A_curr = false;
volatile bool encL_B_curr = false;

volatile long long int count_L = 0;
long long int count_L_n = 0;
double th_L = 0;

uint64_t t_prev = 0;
double th_L_prev = 0;
double w_L = 0;

void setup()
{
  Serial.begin(115200);

  pinMode(ML_1_PIN, OUTPUT);
  pinMode(ML_2_PIN, OUTPUT);
  pinMode(ML_PWM_PIN, OUTPUT);

  pinMode(ENCL_A_PIN, INPUT);
  pinMode(ENCL_B_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCL_A_PIN), isrENCL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCL_B_PIN), isrENCL, CHANGE);

  t_prev = micros();
}

void loop()
{
  if (Serial.available()>0)
  {
    String s = Serial.readString();
    pwm_L = s.toInt();
  }
  
  pwm_L = constrain(pwm_L, -255, 255);

  // Serial.println(pwm_L);

  if (pwm_L>=0)
  {
    digitalWrite(ML_1_PIN, HIGH);
    digitalWrite(ML_2_PIN, LOW);
    analogWrite(ML_PWM_PIN, pwm_L);
  }
  else
  {
    digitalWrite(ML_1_PIN, LOW);
    digitalWrite(ML_2_PIN, HIGH);
    analogWrite(ML_PWM_PIN, -pwm_L);
  }

  noInterrupts();
  count_L_n = count_L;
  interrupts();

  th_L = 360.0 * (double)count_L_n / (double)ENC_CPR;
  uint64_t t_now = micros();
  w_L = 1.0e6 * (th_L - th_L_prev)/(double)(t_now - t_prev);

  t_prev = t_now;
  th_L_prev = th_L;

  // Serial.println(th_L);
  Serial.println(w_L);

  delay(5);
}

void isrENCL()
{
  encL_A_curr = digitalRead(ENCL_A_PIN);
  encL_B_curr = digitalRead(ENCL_B_PIN);

  if(encL_A_curr != encL_A_prev)
  {
    if(encL_A_curr != encL_B_curr)
    {
      count_L++;
    }
    else
    {
      count_L--;
    }
  }
  else
  {
    if(encL_A_curr != encL_B_curr)
    {
      count_L--;
    }
    else
    {
      count_L++;
    }
  }

  // if(count_L>ENC_CPR)
  // {
  //   count_L = count_L - ENC_CPR;
  // }

  encL_A_prev = encL_A_curr;
  encL_B_prev = encL_B_curr;
}
