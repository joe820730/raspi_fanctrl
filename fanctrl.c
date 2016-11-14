#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include "readCpuTemp.h"
#define FAN_PIN 22
#define SPEED_MAX 100

int main()
{
  wiringPiSetupGpio();
  double cpuTemp;
  int readtmp;

  pinMode(FAN_PIN, PWM_OUTPUT);
  int iResult;
  int speed;
  iResult = softPwmCreate(FAN_PIN ,0 ,SPEED_MAX);
  if(iResult!=0)
  {
    printf("Fan initial fail, exit.\n");
    return -1;
  }
  digitalWrite(FAN_PIN, LOW);
  while(1)
  {
    readtmp = readCpuTemp(&cpuTemp);
    if (cpuTemp > 45 && readtmp <= 55)
    {
      softPwmWrite(FAN_PIN,30);
    }
    else if (cpuTemp > 55 && readtmp <= 65)
    {
      softPwmWrite(FAN_PIN,60);
    }
    else if (cpuTemp > 65 && readtmp <= 75)
    {
      softPwmWrite(FAN_PIN,80);
    }
    else if (cpuTemp > 75)
    {
      softPwmWrite(FAN_PIN,100);
    }
    else
    {
      digitalWrite(FAN_PIN,LOW);
    }
    printf("CPUTEMP = %.3lf",cpuTemp);
    usleep(1000000);
    /*printf("CPUTEMP: %lf  Input fan speed (0-%d): ",cpuTemp,SPEED_MAX);
    scanf("%d",&speed);
    if(speed > SPEED_MAX)
    {
      break;
    }
    else
    {
      softPwmWrite(FAN_PIN,speed);
    }*/
  }
  return 0;
}
