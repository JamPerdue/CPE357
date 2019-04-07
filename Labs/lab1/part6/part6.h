#ifndef PART5_H
#define PART5_H

struct point
{
   double x;
   double y;
};
struct Rectangle
{
	struct point top_left;
	struct point bottom_right;
};
int is_a_square(struct Rectangle);
struct point create_point(double x, double y);

#endif
