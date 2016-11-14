OBJECTS = fanctrl.o readCpuTemp.o
OBJ    := $(foreach obj, $(OBJECTS), $(obj))

TARGET := fanctrl.exe
FLAGS  := -pthread -Wall -g -O3
LIBS   := -lwiringPi -lm
CC      = gcc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(FLAGS) $(LIBS)

clean:
	rm -f *.o *.exe

.PHONY: all clean
