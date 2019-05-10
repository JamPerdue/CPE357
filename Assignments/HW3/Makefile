#kefile
  # Symbolic name definitions
  
CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm -g
MAIN = hencode
OBJS = hencode.o
  
   # Rules
  #
 
all: 
	${CC} ${CFLAGS} hencode.c hencode.h -o hencode
	${CC} ${CFLAGS} hdecode.c hdecode.h -o hdecode

clean :
	 rm *.o $(MAIN) core

