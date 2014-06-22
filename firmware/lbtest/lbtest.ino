#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


volatile unsigned long t0 = 0;
volatile unsigned long t1 = 0;
volatile bool updated = false;


void setup()
{
  /*
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  */
  attachInterrupt(0, mt0, FALLING);
  attachInterrupt(1, mt1, FALLING);
  
  Serial.begin(9600);
}

void loop()
{
  if(updated)
  {
    updated = false;
    unsigned long d=0;
    unsigned int ro=0;
    if(t0<t1)
    {
      d = t1-t0;
    }
    else
    {
      ro = 1;
      d = ((unsigned long)-1)-t1+t0;
    }
    p("%lu %lu %lu %d\n", t0, t1, d, ro);
  }
}

void mt0()
{
  t0 = micros();
}

void mt1()
{
  t1 = micros();
  updated = true;
}
