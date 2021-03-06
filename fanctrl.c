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
#define CONFIG_FILE "config.ini"
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
  memset(&fancfg, 0, sizeof(fancfg));
  double cpuTemp;
  int readtmp;
  int ret;
  if(ReadCfgFile(&fancfg, CONFIG_FILE) == -1)
  {
    printf("Config file error, exit!\n");
    return -1;
  }
  printf("Usage: %s [-t for test mode]\n", argv[0]);
#ifdef DEBUG
  printf("cfg: pin=%d, disabletemp=%d, t0=%d, t1=%d, t2=%d, t3=%d\nspeed1=%d, speed2=%d, speed3=%d, speed_max=%d\n",fancfg.fan_pin,fancfg.disableTemp,fancfg.temp[0],fancfg.temp[1],fancfg.temp[2],fancfg.temp[3],fancfg.speed[0],fancfg.speed[1],fancfg.speed[2],fancfg.speed[3]);
#endif

  wiringPiSetupGpio();
  pinMode(fancfg.fan_pin, PWM_OUTPUT);
  ret = softPwmCreate(fancfg.fan_pin ,0 ,SPEED_MAX);
  if(ret < 0)
  {
    printf("Fan initial fail, exit.\n");
    return -1;
  }
  if(argc == 2)
  {
    if(strcmp("-t", argv[1]) == 0)
    {
      int speed = 0;
      printf("Speed test mode\n");
      while(!isHalt)
      {
        readtmp = ReadCpuTemp(&cpuTemp);
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
  }
  else
  {
    while(!isHalt)
    {
      readtmp = ReadCpuTemp(&cpuTemp);
      if (cpuTemp > fancfg.temp[0] && readtmp <= fancfg.temp[1])
      {
        softPwmWrite(fancfg.fan_pin,fancfg.speed[0]);
      }
      else if (cpuTemp > fancfg.temp[1] && readtmp <= fancfg.temp[2])
      {
        softPwmWrite(fancfg.fan_pin,fancfg.speed[1]);
      }
      else if (cpuTemp > fancfg.temp[2] && readtmp <= fancfg.temp[3])
      {
        softPwmWrite(fancfg.fan_pin,fancfg.speed[2]);
      }
      else if (cpuTemp > fancfg.temp[3])
      {
        softPwmWrite(fancfg.fan_pin,fancfg.speed[3]);
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

  softPwmStop(fancfg.fan_pin);
  return 0;
}
