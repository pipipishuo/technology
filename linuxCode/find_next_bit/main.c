#include<stdio.h>
#include<stdlib.h>
#define BITS_PER_LONG 64
#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
static unsigned int generic___ffs(unsigned long word)
{
	unsigned int num = 0;

#if BITS_PER_LONG == 64
	if ((word & 0xffffffff) == 0) {
		num += 32;
		word >>= 32;
	}
#endif
	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return num;
}
#define FIND_NEXT_BIT(FETCH, MUNGE, size, start)				\
({										\
	unsigned long mask, idx, tmp, sz = (size), __start = (start);		\
										\
	if (__start >= sz)						\
		goto out;							\
										\
	mask = MUNGE(BITMAP_FIRST_WORD_MASK(__start));				\
	idx = __start / BITS_PER_LONG;						\
										\
	for (tmp = (FETCH) & mask; !tmp; tmp = (FETCH)) {			\
		if ((idx + 1) * BITS_PER_LONG >= sz)				\
			goto out;						\
		idx++;								\
	}									\
										\
	sz = min(idx * BITS_PER_LONG + __ffs(MUNGE(tmp)), sz);			\
out:										\
	sz;									\
})
int main(){
    long* addr=(long*)malloc(5*BITS_PER_LONG);
    in= start=0;
    long nbits=5*BITS_PER_LONG;
    while(start<=5*BITS_PER_LONG){
        start=FIND_NEXT_BIT(~addr[idx], /* nop */, nbits, start);
        printf("start:%d\n",start);
    }
    return 0;
}
