#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}

#define ASZ(x) (sizeof(x)/sizeof(*x))
#define STAGES (2)
#define LBS (2)

volatile unsigned long autoOffCnt[STAGES] = {0};
volatile unsigned char s1powerOn = 0;
unsigned char stagePin[LBS] = {11, 12};
unsigned char signalPin[LBS] = {13, 13};

void triggerOn(unsigned char stage)
{
  digitalWrite(stagePin[stage],HIGH);
  digitalWrite(signalPin[stage],HIGH);
  autoOffCnt[stage] = 40;
}

void triggerOff(unsigned char stage)
{
  digitalWrite(stagePin[stage],LOW);
  digitalWrite(signalPin[stage],LOW);
  autoOffCnt[stage] = 0;
}

void setup()
{
  for(unsigned char i=0; i<LBS; ++i)
  {
    pinMode(stagePin[i],OUTPUT);
    pinMode(signalPin[i],OUTPUT);
    triggerOff(i);
  }

  attachInterrupt(0, lb0, FALLING);
  attachInterrupt(1, lb1, RISING);
  
  Serial.begin(9600);
}


void lb0()
{
  // 1->0
  //  kill stage 0
  triggerOff(0);
}

void lb1()
{
  //  kill stage 1
  s1powerOn = 6;
  //triggerOn(1);
}


void loop()
{
  // wenn newline
  // trigger shit
  if(Serial.available() && Serial.read() == '\n')
  {
    triggerOn(0);
  }
  
  if(s1powerOn > 0)
  {
    if(--s1powerOn == 0)
    {
      triggerOn(1);
    }
  }
  // hard limit power off
  for(unsigned char i=0; i<STAGES; ++i)
  {
    if(autoOffCnt[i] > 0)
    {
      if(--autoOffCnt[i] == 0)
      {
        triggerOff(i);
      }
    } 
  }
  
  delay(1);
}


