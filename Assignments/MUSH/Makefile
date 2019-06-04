# Makefile
# Symbolic name definitions

CC = gcc
CFLAGS = -Wall -lm -g 
MAIN = mush
OBJS = mush.o

# Rules
#

all:$(MAIN)

$(MAIN) :$(OBJS) mush.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mush.o : mush.c mush.h
	$(CC) $(CFLAGS) -c mush.c

clean :
	rm *.o $(MAIN) core
