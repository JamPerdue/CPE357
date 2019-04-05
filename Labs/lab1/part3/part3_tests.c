#include <stdio.h>

#include "checkit.h"
#include "part3.h"

void test_sum_1()
{
        int arr[5]= {1,2,3,4,5};
   checkit_int(sum(arr,5), 15);
}

void test_sum_2()
{
        int arr[4] = {5,6,7,8};
   checkit_int(sum(arr,4), 26);
}
void test_sum_3()
{
        int  arr[3] = {1,2,3};
   checkit_int(sum(arr,0), 0);
}
void test_sum()
{
   test_sum_1();
   test_sum_2();
   test_sum_3();
}

int main(void)
{
   test_sum();

   return 0;
}
