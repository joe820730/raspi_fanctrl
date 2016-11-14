OBJECTS = fanctrl.o readCpuTemp.o
OBJ    := $(foreach obj, $(OBJECTS), $(obj))

TARGET := fanctrl.exe
FLAGS  := -lwiringPi -pthread -lm -Wall -g -O3
CC      = gcc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(FLAGS)

clean:
	rm -f *.o *.exe

.PHONY: all clean
