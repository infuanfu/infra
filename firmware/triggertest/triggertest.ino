#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


volatile unsigned char autoOffCnt = 0;

void setup()
{
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  attachInterrupt(0, pon, FALLING);
  attachInterrupt(1, poff, FALLING);
  
  Serial.begin(9600);
}

void loop()
{
  if(autoOffCnt > 0)
  {
    if(--autoOffCnt == 0)
    {
      digitalWrite(13,LOW);
    }
  }
  
  delay(1);
}

void pon()
{
  digitalWrite(13,HIGH);
  autoOffCnt = 22;
}

void poff()
{
  digitalWrite(13,LOW);
}
