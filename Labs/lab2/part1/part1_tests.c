#include "part1.h"
#include <stdio.h>
#include "checkit.h"


void test_swap_1(){
	int x = 0;
	int y = 10;
	swap(&x,&y);
	checkit_int(x,10);


}

void test_swap_2(){	
	int h = 100;
	int g = 1020;
	swap(&g,&h);
	checkit_int(g,100);
}

int main(void){
	test_swap_1();
	test_swap_2();
	return 0;
	}
