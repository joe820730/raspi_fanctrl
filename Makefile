DIR = obj
OBJECTS = fanctrl.o readCpuTemp.o readcfg.o
OBJ := $(foreach obj, $(OBJECTS), $(DIR)/$(obj))

TARGET := fanctrl.exe
FLAGS  := -pthread -Wall -g -O3 #-DDEBUG
LIBS   := -lwiringPi -lm
CC = gcc

vpath %.c ./

all: create_folder $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(FLAGS) $(LIBS)

$(DIR)/%.o: %.c
	$(CC) -o $@ -c $^ $(FLAGS)

create_folder:
	mkdir -p $(DIR)

clean:
	rm -f $(DIR)/*.o *.exe

.PHONY: all clean create_folder
