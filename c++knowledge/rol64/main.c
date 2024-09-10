#include<stdio.h>
static inline long count_masked_bytes(unsigned long mask)
{
	return mask*0x0001020304050608ul >> 56;
}
static inline unsigned long create_zero_mask(unsigned long bits)
{
	bits = (bits - 1) & ~bits;
	return bits >> 7;
}
int main(){
	long a=0x80;
	long y=create_zero_mask(a);
	printf("res:%ld\n",y);
	return 0;
}
