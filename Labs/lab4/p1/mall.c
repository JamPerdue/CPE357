#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


void *malloc(size_t size){
	void *p = sbrk(0);
  void *request = sbrk(size);
  if (request == (void*) -1) {
    return NULL; // sbrk failed.
  } else {
    
    return p;
  }
}
int main(int argc, char * argv[]){
	int *ptr = malloc(10);

	printf("success\n");






}
