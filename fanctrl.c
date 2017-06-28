#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "readCpuTemp.h"
#include "fanctrl.h"
#include "readcfg.h"
bool isHalt = false;

void sigHandle(int signal)
{
  isHalt = true;
  printf("Program exit.\n");
}

int main(int argc, char **argv)
{
  signal(SIGINT,sigHandle);
  FANConfig fancfg;
  double cpuTemp;
  int readtmp;
  int ret;
  if(argc >= 2)
  {
    readCfgFile(&fancfg, argv[1]);
  }
  else
  {
    printf("Usage: %s [configFile] [-t for test mode]\n", argv[0]);
    return 0;
  }
#ifdef DEBUG
  printf("CFG: pin=%d, disableTemp=%d, t0=%d, t1=%d, t2=%d, t3=%d\nspeed1=%d, speed2=%d, speed3=%d, speed_max=%d\n",fancfg.fan_pin,fancfg.disableTemp,fancfg.temp0,fancfg.temp1,fancfg.temp2,fancfg.temp3,fancfg.speed1,fancfg.speed2,fancfg.speed3,fancfg.speed_max);
#endif

  wiringPiSetupGpio();
  pinMode(fancfg.fan_pin, PWM_OUTPUT);
  ret = softPwmCreate(fancfg.fan_pin ,0 ,SPEED_MAX);
  if(ret < 0)
  {
    printf("Fan initial fail, exit.\n");
    return -1;
  }
  if(argc == 3)
  {
    if(strcmp("-t", argv[2]) == 0)
    {
      int speed = 0;
      printf("Speed test mode\n");
      while(!isHalt)
      {
        readtmp = readCpuTemp(&cpuTemp);
        printf("CPUTEMP: %lf  Input fan speed (0-%d): ",cpuTemp,SPEED_MAX);
        scanf("%d",&speed);
        if(speed > SPEED_MAX || speed < 0)
        {
          softPwmWrite(fancfg.fan_pin,0);
          softPwmStop(fancfg.fan_pin);
          return 0;
        }
        else
        {
          softPwmWrite(fancfg.fan_pin,speed);
        }
      }
    }
    else
    {
      while(!isHalt)
      {
        readtmp = readCpuTemp(&cpuTemp);
        if (cpuTemp > fancfg.temp0 && readtmp <= fancfg.temp1)
        {
          softPwmWrite(fancfg.fan_pin,fancfg.speed1);
        }
        else if (cpuTemp > fancfg.temp1 && readtmp <= fancfg.temp2)
        {
          softPwmWrite(fancfg.fan_pin,fancfg.speed2);
        }
        else if (cpuTemp > fancfg.temp2 && readtmp <= fancfg.temp3)
        {
          softPwmWrite(fancfg.fan_pin,fancfg.speed3);
        }
        else if (cpuTemp > fancfg.temp3)
        {
          softPwmWrite(fancfg.fan_pin,fancfg.speed_max);
        }
        else if (cpuTemp < fancfg.disableTemp)
        {
          softPwmWrite(fancfg.fan_pin,0);
        }
#ifdef DEBUG
        printf("CPUTEMP = %.3lf\n",cpuTemp);
#endif
        usleep(2000000);
      }
    }
  }
  softPwmStop(fancfg.fan_pin);
  return 0;
}
