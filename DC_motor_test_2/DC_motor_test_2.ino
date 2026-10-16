#define ENCL_A_PIN 2
#define ENCL_B_PIN 3

int pos_n = 0;

volatile int pos = 0;
volatile int a_curr = false;
volatile int a_prev = false;
volatile int b_curr = false;
volatile int b_prev = false;

void setup() {
  Serial.begin(9600);
  pinMode(ENCL_A_PIN, INPUT);
  pinMode(ENCL_B_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCL_A_PIN), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCL_B_PIN), readEncoder, CHANGE);
}

void loop() {
  noInterrupts();
  pos_n = pos;
  interrupts();

  Serial.println(pos_n);
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