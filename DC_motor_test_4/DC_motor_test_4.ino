#define ENCL_A_PIN 2
#define ENCL_B_PIN 3
#define ML_1_PIN 4
#define ML_2_PIN 5
#define ML_PWM_PIN 6
#define CPR 5800
#define PI 3.1415926

int pwm_L = 0;
lomg long int pos_n = 0;
double th_L = 0;
double th_L_prev = 0;
uint64_t t = 0;
uint64_t t_prev = 0;
double dt = 0.0;
double w_L = 0;
double w_L_filt = 0;
double w_L_filt_prev = 0;
double alpha = 0.0;
double fc = 10;
double tau = 1.0/(2.0*PI*fc);

volatile long long int pos = 0;
volatile int a = false;
volatile int a_prev = false;
volatile int b = false;
volatile int b_prev = false;

void setup()
{
  Serial.begin(115200);
  pinMode(ML_1_PIN, OUTPUT);
  pinMode(ML_2_PIN, OUTPUT);
  pinMode(ML_PWM_PIN, OUTPUT);
  pinMode(ENCL_A_PIN, INPUT);
  pinMode(ENCL_B_PIN, INPUT);

  t_prev = micros();

  attachInterrupt(digitalPinToInterrupt(ENCL_A_PIN), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCL_B_PIN), readEncoder, CHANGE);
}

void loop()
{
  if (Serial.available()>0)
  {
    String s = Serial.readString();
    pwm_L = s.toInt();
  }

  pwm_L = constrain(pwm_L, -255, 255);

  if(pwm_L>0)
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

  noInterrupts();
  pos_n = pos;
  interrupts();

  th_L = 2.0 * PI * (double)pos_n / (double)CPR;

  t = micros();
  dt = (double)(t - t_prev)/1.0e6;
  w_L = (th_L - th_L_prev)/dt;
  alpha = dt/(tau + dt);
  w_L_filt = alpha*w_L + (1.0 - alpha)*w_L_filt_prev;

  th_L_prev = th_L;
  t_prev = t;
  w_L_filt_prev = w_L_filt;

  Serial.print(w_L);
  Serial.print(",");
  Serial.println(w_L_filt);

  delay(5);
}

void readEncoder()
{
  a = digitalRead(ENCL_A_PIN);
  b = digitalRead(ENCL_B_PIN);

  if(a != a_prev)
  {
    // interrupt due to A
    if(a != b)
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
    //interrupt due to B
    if(a == b)
    {
      pos++;
    }
    else
    {
      pos--;
    }
  }

  a_prev = a;
  b_prev = b;
}