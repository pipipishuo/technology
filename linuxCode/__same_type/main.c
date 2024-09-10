#include<stdio.h>
struct test{
	int a;
	double b;
};
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
int main(){
	_Static_assert(__same_type(int, ((struct test *)0)->a),"compatible_p");
	
	
	return 0;
}
