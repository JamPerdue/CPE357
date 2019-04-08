#include "part4.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void str_lower(char *orig, char *buffer){
        int i = 0;
        int s = strlen(orig);

        while(i < s){
                buffer[i] = tolower(orig[i]);
                i++;
        }
	buffer[i] = 0;/*
        int i, s = strlen(orig);
        for (i = 0; i < s; i++){
            buffer[i] = tolower(orig[i]);
    }*/

}

void str_lower_mutate(char *orig){
        int i = 0;
        char *temp = orig;
        while(orig[i]){
                orig[i] = tolower(temp[i]);
                i++;
        }
        orig[i] = 0;




}
