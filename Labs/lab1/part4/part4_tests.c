#include <stdio.h>

#include "checkit.h"
#include "part4.h"

void test_str_lower_1()
{
        char buffer[6];
        char *orig = "Hello";
        char *soln = "hello";
        str_lower(orig,buffer);
        checkit_string(buffer, soln);
}

void test_str_lower_2()
{
        char  buffer[20];
        char *orig = "HOLL,,, Hello World";
        char *soln = "holl,,, hello world";
        str_lower(orig,buffer);
        checkit_string(buffer, soln);
}
void test_str_lower_3()
{
        char orig[] = "HelLO";
        char *soln = "hello";
        str_lower_mutate(orig);
        checkit_string(orig, soln);
}
void test_str_lower_4()
{
        char orig[] = "";
        char *soln = "";
        str_lower_mutate(orig);
        checkit_string(orig, soln);
}
void test_str_lower()
{
   test_str_lower_1();
   test_str_lower_2();
   test_str_lower_3();
   test_str_lower_4();
}

int main(void)
{
   test_str_lower();

   return 0;
}
