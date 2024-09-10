# define __user		__attribute__((user))
# define __kernel		__attribute__((noderef, address_space(0)))

int add(char __user *d){
    return 0;
}
int main(){
	char __user * d;
    add(d);
    return 0;
}
