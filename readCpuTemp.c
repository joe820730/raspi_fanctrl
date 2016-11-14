#include <fcntl.h>
#include <stdio.h>

int readCpuTemp(double *cpuTemp)
{
  int fd;
  char cpuTempStr[8];
  int temp;
  int cpuTempRead;

  fd = open("/sys/class/thermal/thermal_zone0/temp",O_RDONLY);
  if(fd < 0)
  {
    printf("Failed to open thermal_zone/temp.\n");
    return -1;
  }
  cpuTempRead = read(fd, cpuTempStr, 8);
  if(cpuTempRead < 0)
  {
    printf("Failed to read cpu temperature.\n");
    return -1;
  }

  temp = atoi(cpuTempStr);
  *cpuTemp = (double)temp / 1000.0;
  close(fd);
  return 0;
}
