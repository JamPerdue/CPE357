#include "part6.h"

struct point create_point(double x, double y)
{
   struct point p = {x, y};
   return p;
}

int is_a_square(struct Rectangle r){
	if((r.bottom_right.x-r.top_left.x)==(r.top_left.y-r.bottom_right.y)){
		return 1;
	}
	return 0;


