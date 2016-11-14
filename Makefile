OBJECTS = fanctrl.o readCpuTemp.o readcfg.o
OBJ := $(foreach obj, $(OBJECTS), $(obj))

TARGET := fanctrl.exe
FLAGS  := -pthread -Wall -g -O3 -DDEBUG
LIBS   := -lwiringPi -lm
CC = gcc

vpath %.c ./

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(FLAGS) $(LIBS)

%.o: %.c
	$(CC) -o $@ -c $^ $(FLAGS)

clean:
	rm -f *.o *.exe

.PHONY: all clean
