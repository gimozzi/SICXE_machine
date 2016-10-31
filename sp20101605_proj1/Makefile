#20101605 SPprj1
#Makefile


##################################################
CC = gcc
CFLAGS = -W -Wall -g -O2
TARGET = 20101605.out
OBJECTS = init.o input.o list.o command.o errorhandling.o
MAIN = 20101605.c

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(MAIN)

.c.o:
	$(CC) $(CLFAGS) -c $<

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

