# define __user		__attribute__((user))
# define __kernel		__attribute__((noderef, address_space(0)))
#define O_ACCMODE	00000003
typedef unsigned int fmode_t;
#define FMODE_NONOTIFY		((fmode_t)(1 << 26))
#define OPEN_FMODE(flag) ((unsigned int)(((flag + 1) & O_ACCMODE) | \
					    (flag &FMODE_NONOTIFY)))
int add(char __user *d){
    return 0;
}
int main(){
	
    int s=OPEN_FMODE(1);
    printf("%x\n",s);
    return 0;
}
