#define SPEED_MAX 100

typedef struct {
  int fan_pin;
  int disableTemp;
  int temp[4];
  int speed[4];
} FANConfig;
