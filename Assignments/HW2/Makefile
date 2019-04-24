# Makefile
# Symbolic name definitions

CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm -g 
MAIN = fw
OBJS = fw.o

# Rules
#

all:$(MAIN)

$(MAIN) :$(OBJS) fw.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

fw.o : fw.c fw.h
	$(CC) $(CFLAGS) -c fw.c

clean :
	rm *.o $(MAIN) core
