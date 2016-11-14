#define SPEED_MAX 100

typedef struct {
  int fan_pin;
  int disableTemp;
  int temp0;
  int temp1;
  int temp2;
  int temp3;
  int speed1;
  int speed2;
  int speed3;
  int speed_max;
} fanconfig;

#ifndef _READCFG_H_INCLUDED_
#define _READCFG_H_INCLUDED_
extern int readCfgFile(fanconfig *fancfg);
#endif
