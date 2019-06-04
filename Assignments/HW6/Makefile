# Makefile
# Symbolic name definitions

CC = gcc
CFLAGS = -Wall -lm -g 
MAIN = parseline
OBJS = parseline.o

# Rules
#

all:$(MAIN)

$(MAIN) :$(OBJS) parseline.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

parseline.o : parseline.c parseline.h
	$(CC) $(CFLAGS) -c parseline.c

clean :
	rm *.o $(MAIN) core
