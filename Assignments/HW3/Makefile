#kefile
  # Symbolic name definitions
  
CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm -g
MAIN = hencode
OBJS = hencode.o, hdecode.o
  
   # Rules
  #
 
all:$(MAIN)
 
$(MAIN) :$(OBJS) hencode.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)
 
#$(MAIN) :$(OBJS) hdecode.h
#	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

hencode.o : hencode.c hencode.h
	$(CC) $(CFLAGS) -c hencode.c
 
hdecode.o : hdecode.c hdecode.h
	$(CC) $(CFLAGS) -c hdecode.c

clean :
	 rm *.o $(MAIN) core

