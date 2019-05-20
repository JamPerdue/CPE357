# Makefile
# Symbolic name definitions

CC = gcc
CFLAGS = -Wall -pedantic -lm -g 
MAIN = mytar
OBJS = mytar.o

# Rules
#

all:$(MAIN)

$(MAIN) :$(OBJS) mytar.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mytar.o : mytar.c mytar.h
	$(CC) $(CFLAGS) -c mytar.c

clean :
	rm *.o $(MAIN) core
