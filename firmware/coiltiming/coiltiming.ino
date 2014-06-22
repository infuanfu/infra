#include <stdarg.h>

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}


#define TRIGGERTIME (20)
#define LBS (4)
volatile unsigned long time[LBS] = {0};
volatile bool hot = false;
volatile bool triggered = false;

volatile unsigned char autoOffCnt = 0;

void setup()
{
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  attachInterrupt(0, lb0, FALLING); // 3
  attachInterrupt(1, lb2, FALLING); // 2
  attachInterrupt(2, lb1, FALLING); // 0
  attachInterrupt(3, lb3, FALLING); // 1
                                    // 7
  
  Serial.begin(9600);
  Serial.write("hello\n");
}

unsigned long delta(unsigned long a, unsigned long b)
{
  return b-a;
}

unsigned char cmd = 0;
void loop()
{
  if(autoOffCnt > 0)
  {
    if(--autoOffCnt == 0)
    {
      digitalWrite(12,LOW);
    }
  }
  else if(Serial.available())
  {
    unsigned char t = Serial.read();
    switch(t)
    {
    case '\n':
      p("exe: %c\n",cmd);
      switch(cmd)
      {
      case 's':
        hot = false;
        triggered = false;
        break;
      case 't':
        lb0();
        break;
      case 'h':
        hot = true;
        triggered = false;
        time[0] = 0;
        time[1] = 0;
        time[2] = 0;
        time[3] = 0;
        break;
      case 'd':
        p("%lu <%lu> %lu <%lu> %lu <%lu> %lu   %c%c\n",time[0], delta(time[0],time[1]),
                                                time[1], delta(time[1],time[2]),
                                                time[2], delta(time[2],time[3]),
                                                time[3],
                                                hot ? 'H' : 'h',
                                                triggered ? 'T' : 't');
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
  if(hot)
  {
    hot = false;
    triggered = true;
    autoOffCnt = TRIGGERTIME+1;
    digitalWrite(12,HIGH);  
    time[0] = micros();
  }
}

void lb1()
{
  time[1] = micros();
}

void lb2()
{
  time[2] = micros();
}

void lb3()
{
  time[3] = micros();
}


