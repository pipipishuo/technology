#include<stdio.h>
#include <fcntl.h>
#define WILL_CREATE(flags)	(flags & (O_CREAT | __O_TMPFILE))
int main(){
    //int fd=open("/home/pipishuo/Desktop",O_NOFOLLOW);
    int fd=open("/home/pipishuo/Desktop/linux",O_NOFOLLOW);
   printf("fd:%d\n",fd);
    return 0;
    }
