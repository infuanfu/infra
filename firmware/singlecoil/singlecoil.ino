#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


#define TRIGGERTIME1 (16)
volatile unsigned char triggerTime0 = TRIGGERTIME1;
volatile unsigned char triggerTime1 = 20;
volatile unsigned char autoOffCnt0 = 0;
volatile unsigned char autoOffCnt1 = 0;

volatile unsigned long times[2]={0};

void setup()
{
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  attachInterrupt(0,lb0,CHANGE);
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
      case '+':
        ++triggerTime0;
        p("triggertime0: %d ms\n", triggerTime0);
        break;
      case '-':
        --triggerTime0;
        p("triggertime0: %d ms\n", triggerTime0);
        break;
      case '>':
        ++triggerTime1;
        p("triggertime1: %d x\n", triggerTime1);
        break;
      case '<':
        --triggerTime1;
        p("triggertime1: %d x\n", triggerTime1);
        break;
      case '?':
      {
        unsigned long delta = times[1] - times[0];
        p("triggertime0: %d ms\ntriggertime1: %d x\n%ld %ld %ld\n", triggerTime0,triggerTime1,times[0], times[1], delta);
      }  break;
      case 't':
        autoOffCnt0 = triggerTime0+1;
        p("triggered with %d ms\n", triggerTime0);
        digitalWrite(12,HIGH);
        break;
      default:
        p("%s\n","unknown command");
      }
      break;
    default:
      cmd = t;
    }
  }
  
  delay(1);
}


void lb0()
{
  if(digitalRead(3) == HIGH)
  {
    times[1] = micros();
    
    unsigned long d = times[1]-times[0];
    
    unsigned long off = (d*20)/1000;
    autoOffCnt1 = off;    
    digitalWrite(11,HIGH);
 }
  else
  {
    times[0] = micros();
  }
}
