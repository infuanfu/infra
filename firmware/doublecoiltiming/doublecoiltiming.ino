#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


#define TRIGGERTIME1 (11)
#define TRIGGERTIME2 (11)

volatile unsigned char autoOffCnt0 = 0;
volatile unsigned char autoOffCnt1 = 0;

void setup()
{
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  attachInterrupt(0, lb0, FALLING); // 3
  attachInterrupt(1, lb1, FALLING); // 2
  
  Serial.begin(9600);
}

unsigned char cmd = 0;
void loop()
{
  bool busy = false;
  
  if(autoOffCnt0 > 0)
  {
    if(--autoOffCnt0 == 0)
    {
      digitalWrite(12,LOW);
    }
    busy = true;
  }

  if(autoOffCnt1 > 0)
  {
    if(--autoOffCnt1 == 0)
    {
      digitalWrite(11,LOW);
    }
    busy = true;
  }

  if(!busy && Serial.available())
  {
    unsigned char t = Serial.read();
    switch(t)
    {
    case '\n':
      switch(cmd)
      {
      case 't':
        autoOffCnt0 = TRIGGERTIME1+1;
        digitalWrite(12,HIGH);
        break;
      default:
        p("%s\n","unknown command");
      }
      break;
    default:
      cmd = t;
      p("cmd: %c\n",cmd);
    }
  }
  
  delay(1);
}

void lb0()
{
}

void lb1()
{
  digitalWrite(11,HIGH);
  autoOffCnt1 = TRIGGERTIME2+1;
}



