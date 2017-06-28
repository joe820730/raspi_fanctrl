#include <stdio.h>
#include <stdlib.h>
#include "fanctrl.h"

void readCfgFile(FANConfig *fancfg, const char *CFGFILE)
{
  FILE *cfg = fopen(CFGFILE,"r");
  if(cfg != NULL)
  {
    fscanf(cfg,"PIN=%d\n",&fancfg->fan_pin);
    fscanf(cfg,"DisableTemp=%d\n",&fancfg->disableTemp);
    fscanf(cfg,"Temp=%d, %d, %d, %d\n",&fancfg->temp0,&fancfg->temp1,&fancfg->temp2,&fancfg->temp3);
    fscanf(cfg,"Speed=%d, %d, %d, %d",&fancfg->speed1,&fancfg->speed2,&fancfg->speed3,&fancfg->speed_max);
  }
  else
  {
    printf("Cannot open %s, use default setting.\n", CFGFILE);
    fancfg->fan_pin = 27;
    fancfg->disableTemp = 50;
    fancfg->temp0 = 53;
    fancfg->temp1 = 57;
    fancfg->temp2 = 61;
    fancfg->temp3 = 65;
    fancfg->speed1 = 50;
    fancfg->speed2 = 65;
    fancfg->speed3 = 80;
    fancfg->speed_max = SPEED_MAX;
  }
}
