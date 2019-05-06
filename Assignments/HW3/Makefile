#kefile
  # Symbolic name definitions
  
CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm -g
MAIN = hencode
OBJS = hencode.o
  
   # Rules
  #
 
all:$(MAIN)
 
$(MAIN) :$(OBJS) hencode.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)
 
hencode.o : hencode.c hencode.h
	$(CC) $(CFLAGS) -c hencode.c
 
clean :
	 rm *.o $(MAIN) core

