#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


volatile unsigned long autoOffCnt = 0;

void triggerOn()
{
  digitalWrite(13,HIGH);
  digitalWrite(12,HIGH);
}
void triggerOff()
{
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
}

void setup()
{
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  triggerOff();
  attachInterrupt(0, poff, FALLING);
  
  Serial.begin(9600);
}

void loop()
{
  
  if(autoOffCnt > 0)
  {
    if(--autoOffCnt == 0)
    {
      triggerOff();
    }
  }
  else
  {
    if(Serial.available() > 0 && Serial.read() == '\n')
    {
      triggerOn();
      autoOffCnt = 50;
    }
  }  
  delay(1);
}

void poff()
{
  triggerOff();
}
