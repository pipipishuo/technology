#include<stdio.h>
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
static inline unsigned long load_unaligned_zeropad(const void *addr)
{
	unsigned long ret;

	asm volatile(
		"1:	mov %[mem], %[ret]\n"
		"2:\n"
		_ASM_EXTABLE_TYPE(1b, 2b, 20)
		: [ret] "=r" (ret)
		: [mem] "m" (*(unsigned long *)addr));

	return ret;
}
#define REPEAT_BYTE(x)	((~0ul / 0xff) * (x))
int main(){
	char* name="qwer";
	
	//ptr++;
	long y=REPEAT_BYTE(0x80);
	printf("res:%lX\n",y);
	return 0;
}
