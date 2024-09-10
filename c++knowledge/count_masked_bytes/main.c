#include<stdio.h>
struct word_at_a_time {
	const unsigned long one_bits, high_bits;
};
static inline unsigned long rol64(unsigned long word, unsigned int shift)
{
	return (word << (shift & 63)) | (word >> ((-shift) & 63));
}
#define GOLDEN_RATIO_64 0x61C8864680B583EBull
#define HASH_MIX(x, y, a)	\
	(	x ^= (a),	\
	y ^= x,	x = rol64(x,12),\
	x += y,	y = rol64(y,45),\
	y *= 9			)
#define zero_bytemask(mask) (mask)
#define REPEAT_BYTE(x)	((~0ul / 0xff) * (x))
#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)
# define _ASM_EXTABLE_TYPE(from, to, type)			\
	" .pushsection \"__ex_table\",\"a\"\n"			\
	" .balign 4\n"						\
	" .long (" #from ") - .\n"				\
	" .long (" #to ") - .\n"				\
	" .long " __stringify(type) " \n"			\
	" .popsection\n"
	
#define WORD_AT_A_TIME_CONSTANTS { REPEAT_BYTE(0x01), REPEAT_BYTE(0x80) }
static inline unsigned long load_unaligned_zeropad(const void *addr)
{
	unsigned long ret;

	asm volatile(
		"1:	mov %[mem], %[ret]\n"
		"2:\n"
		//_ASM_EXTABLE_TYPE(1b, 2b, EX_TYPE_ZEROPAD)
		: [ret] "=r" (ret)
		: [mem] "m" (*(unsigned long *)addr));

	return ret;
}
/* Return nonzero if it has a zero */
static inline unsigned long has_zero(unsigned long a, unsigned long *bits, const struct word_at_a_time *c)
{
	unsigned long mask = ((a - c->one_bits) & ~a) & c->high_bits;
	*bits = mask;
	return mask;
}
static inline unsigned int fold_hash(unsigned long x, unsigned long y)
{
	y ^= x * GOLDEN_RATIO_64;
	y *= GOLDEN_RATIO_64;
	return y >> 32;
}
static inline long count_masked_bytes(unsigned long mask)
{	
	
	long res=mask*0x0001020304050608ul;
	printf("mask*:%llx\n",res);
	return  res>> 56;
}
static inline unsigned long prep_zero_mask(unsigned long a, unsigned long bits, const struct word_at_a_time *c)
{
	return bits;
}
static inline unsigned long create_zero_mask(unsigned long bits)
{
	bits = (bits - 1) & ~bits;
	return bits >> 7;
}
static inline unsigned long find_zero(unsigned long mask)
{
	return count_masked_bytes(mask);
}
static inline long hash_name(const void *salt, const char *name)
{
	unsigned long a = 0, b, x = 0, y = (unsigned long)salt;
	unsigned long adata, bdata, mask, len;
	const struct word_at_a_time constants = WORD_AT_A_TIME_CONSTANTS;

	len = 0;
	goto inside;

	do {
		HASH_MIX(x, y, a);
		len += sizeof(unsigned long);
		printf("x:%lx\n",x);
		printf("y:%lx\n",y);
inside:
		a = load_unaligned_zeropad(name+len);
		b = a ^ REPEAT_BYTE('/');
	} while (!(has_zero(a, &adata, &constants) | has_zero(b, &bdata, &constants)));

	adata = prep_zero_mask(a, adata, &constants);
	bdata = prep_zero_mask(b, bdata, &constants);
	printf("a:%lx\n",a);
	printf("adata:%lx\n",adata);
	printf("bdata:%ld\n",bdata);
	mask = create_zero_mask(adata | bdata);
	x ^= a & zero_bytemask(mask);
	printf("x:%lx\n",x);
	 unsigned int res11=fold_hash(x, y);
	 
	 printf("fold_hash(x, y):%lx\n",res11);
	printf("mask:%lx\n",mask);
	long res=count_masked_bytes(mask);
	printf("res:%ld\n",res);
	printf("len:%ld\n",len);
	return res;
}
int main(){
	void *ptr=0x123456789;
	char *name="asdasadaaa/dasd/aqwe";
	hash_name(ptr,name);
	
	
	count_masked_bytes(0xffffffffffffff);
	return 0;
}


