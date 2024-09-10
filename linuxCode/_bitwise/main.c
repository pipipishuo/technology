#include<stdio.h>
#include <fcntl.h>
#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
#define __bitwise	__attribute__((bitwise))
#define __force	__attribute__((force))
typedef  int  __bitwise slab_flags_t;
typedef  int  __bitwise fmode_t;
int main(){
   slab_flags_t a;
   fmode_t b;
   a=(__force slab_flags_t)b;
   return 0;
}
