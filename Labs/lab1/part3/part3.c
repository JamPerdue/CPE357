#include "part3.h"




int sum(int * array, int num){
        int tot = 0;
        int i = num-1;
        while(i>-1){
                tot+= array[i];
                i--;
        }
        return tot;



}
