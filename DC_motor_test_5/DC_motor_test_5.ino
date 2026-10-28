#define ENCL_A_PIN 2
#define ENCL_B_PIN 3
#define ML_PWM_PIN 4
#define ML_1_PIN 6
#define ML_2_PIN 5
#define CPR 5800
#define PI 3.1415926

int pwm_L = 0;
long long int pos_n = 0;
double th_L = 0;
double th_L_prev = 0;
uint64_t t = 0;
uint64_t t_prev = 0;
double dt = 0.0;
double w_L = 0;

double w_L_filt = 0;
double w_L_filt_prev = 0;
double alpha = 0.0;
double fc = 12.0;
double tau = 1.0/(2.0*PI*fc);

double w_target = 0.0 * PI / 180.0; // [-620, 620] deg/s
double u = 0.0;
double kp = 42;
double ki = 300; //200
double kd = 0.1;
double e = 0.0;
double e_prev = 0.0;
double e_integral = 0.0;

volatile long long int pos = 0;
volatile int a = false;
volatile int a_prev = false;
volatile int b = false;
volatile int b_prev = false;

double user_input = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ML_1_PIN, OUTPUT);
  pinMode(ML_2_PIN, OUTPUT);
  pinMode(ML_PWM_PIN, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(ML_1_PIN, LOW);
  digitalWrite(ML_2_PIN, LOW);

  pinMode(ENCL_A_PIN, INPUT);
  pinMode(ENCL_B_PIN, INPUT);

  t_prev = micros();

  attachInterrupt(digitalPinToInterrupt(ENCL_A_PIN), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCL_B_PIN), readEncoder, CHANGE);
}

void loop()
{
  noInterrupts();
  pos_n = pos;
  interrupts();

  getActualVelocity();

  // velocity controller
  e = w_target - w_L_filt;
  e_integral = e_integral + ki*e*dt;
  e_integral = constrain(e_integral, -200.0, 200.0);

  u = (int)(kp*e + e_integral + kd*(e - e_prev)/dt);
  u = constrain(u, -250, 250);

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

  // Serial.print(w_L * 180 / PI);
  // Serial.print(",");
  // Serial.println(w_L_filt * 180.0 / PI);

  Serial.print(0);
  Serial.print(",");
  Serial.print(w_target * 180.0 / PI);
  Serial.print(",");
  Serial.print(w_L_filt * 180.0 / PI);
  Serial.print(",");
  Serial.println(u);
  
  delay(1000/250);
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

void getActualVelocity()
{
  th_L     = 2.0* PI * (double)pos_n / (double)CPR;
  t        = micros();
  dt       = (double)(t - t_prev)/1.0e6;
  w_L      = (th_L - th_L_prev)/dt;
  alpha    = dt/(tau + dt);
  w_L_filt = alpha*w_L + (1.0 - alpha)*w_L_filt_prev;

  th_L_prev = th_L;
  t_prev = t;
  w_L_filt_prev = w_L_filt;
}