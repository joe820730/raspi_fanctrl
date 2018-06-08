#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fanctrl.h"

int ReadLine(FILE* fp, char** strptr)
{
  int strpos = 0;
  if((*strptr) != NULL)
  {
    free((*strptr));
    (*strptr) = NULL;
  }
  (*strptr) = calloc(64, sizeof(char));
  while(1)
  {
    int c = fgetc(fp);
    switch(c)
    {
      case '\r':
      {
        int ctmp = fgetc(fp);
        if(ctmp != '\n' && ctmp != EOF)
          fseek(fp, -1, SEEK_CUR);
        return 0;
      }
      case '\n':
        return 0;
      case EOF:
        return EOF;
      default:
      {
        (*strptr)[strpos] = (char)c;
        strpos++;
        if(strpos >= 64)
        {
          return 0;
        }
      }
    }
  }
}

int ReadCfgFile(FANConfig *fancfg, const char *CFGFILE)
{
  FILE *cfg = fopen(CFGFILE,"rb");
  if(cfg != NULL)
  {
    char* tmpLine = NULL;
    while(!feof(cfg))
    {
      if(ReadLine(cfg, &tmpLine) != EOF)
      {
        char* pch = strtok(tmpLine, "=");
        if(strcmp(pch, "PIN") == 0)
        {
          pch = strtok(NULL, "");
          fancfg->fan_pin = atoi(pch);
#ifdef DEBUG
          printf("PIN = %d\n", fancfg->fan_pin);
#endif
        }
        else if(strcmp(pch, "DisableTemp") == 0)
        {
          pch = strtok(NULL, "");
          fancfg->disableTemp = atoi(pch);
#ifdef DEBUG
          printf("DisableTemp = %d\n", fancfg->disableTemp);
#endif
        }
        else if(strcmp(pch, "Temp") == 0)
        {
          for(int i = 0; i < 4; i++)
          {
            pch = strtok(NULL, ",");
            fancfg->temp[i] = atoi(pch);
          }
#ifdef DEBUG
          printf("Temp = ");
          for(int i = 0; i < 4; i++)
            printf("%d, ", fancfg->temp[i]);
          printf("\n");
#endif
        }
        else if(strcmp(pch, "Speed") == 0)
        {
          for(int i = 0; i < 4; i++)
          {
            pch = strtok(NULL, ",");
            fancfg->speed[i] = atoi(pch);
          }
#ifdef DEBUG
          printf("Speed = ");
          for(int i = 0; i < 4; i++)
            printf("%d, ", fancfg->speed[i]);
          printf("\n");
#endif
        }
      }
    }
    fclose(cfg);
  }
  else
  {
    printf("Cannot open %s, use default setting.\n", CFGFILE);
    fancfg->fan_pin = 27;
    fancfg->disableTemp = 50;
    fancfg->temp[0] = 53;
    fancfg->temp[1] = 57;
    fancfg->temp[2] = 61;
    fancfg->temp[3] = 65;
    fancfg->speed[0] = 50;
    fancfg->speed[1] = 65;
    fancfg->speed[2] = 80;
    fancfg->speed[3] = SPEED_MAX;
  }
  if(fancfg->fan_pin > 27)
  {
    printf("FATAL error, please set fan_pin between 0 to 27.\n");
    return -1;
  }
  for(int i = 0; i < 4; i++)
  {
    if(fancfg->temp[i] == 0 || fancfg->speed[i] == 0)
    {
      printf("Please set temp and speed value bigger than 1.\n");
      return -1;
    }
    if(i < 3)
    {
      if(fancfg->temp[i] > fancfg->temp[i+1])
      {
        printf("Temperature range error!\n");
        return -1;
      }
    }
  }
  if(fancfg->disableTemp <= 0 || fancfg->disableTemp > fancfg->temp[0])
  {
    printf("Please set disable temp between 1 to 100 and lower than temp[0].\n");
    return -1;
  }
  return 0;
}
