#include "part4.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void str_lower(char *orig, char *buffer){

        while(*orig){
                 *buffer= tolower(*orig);
                orig++;
		buffer++;
        }
	*buffer = 0;/*
        int i, s = strlen(orig);
        for (i = 0; i < s; i++){
            buffer[i] = tolower(orig[i]);
    }*/

}

void str_lower_mutate(char *orig){
        
        char *temp = orig;
        while(*orig){
                *orig = tolower(*temp);
                orig++;
		temp++;
        }
        *orig = 0;




}
