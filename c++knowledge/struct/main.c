#include<stdio.h>
struct filename {
	const char		iname[];
};
int main(){
    int s=sizeof(struct filename);
    printf("size:%d\n",s);
    return 0;
}
