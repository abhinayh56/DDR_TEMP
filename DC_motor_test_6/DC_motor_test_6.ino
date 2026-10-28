#define ENCL_A_PIN 2
#define ENCL_B_PIN 3
#define pwmA 4
#define Ain1 6
#define Ain2 5
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
double kp = 42.0; //42.0
double ki = 300.0; //300.0
double kd = 0.1; //0.1
double e = 0.0;
double e_prev = 0.0;
double e_integral = 0.0;

volatile long long int pos = 0;
volatile int a = false;
volatile int a_prev = false;
volatile int b = false;
volatile int b_prev = false;

void setup()
{
  Serial.begin(115200);

  // Set all the motor control pins to outputs
  pinMode(pwmA, OUTPUT);

  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(Ain1, LOW);
  digitalWrite(Ain2, LOW);

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
    digitalWrite(Ain1, HIGH);
    digitalWrite(Ain2, LOW);
  }
  else
  {
    digitalWrite(Ain1, LOW);
    digitalWrite(Ain2, HIGH);
  }
  analogWrite(pwmA, abs(u));

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