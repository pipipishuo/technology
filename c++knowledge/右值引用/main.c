#include<stdio.h>
#include<iostream>
class A{
public:
   A(int);
    A(const A &p);
    A& operator = (const A& a);
   // A(A &&p);
    int a;
};
A& A::operator =(const A& a){
    printf("= operator!\n");
    return *this;
}
A::A(int i){
    a=i;
}
A::A(const A &a){
    printf("copy operation!\n");
}
/*A::A(A &&a){
    printf("move operation!\n");
}*/

A getA(){
    A a(3);
    return a;
}
int main(){
   A a(0);
   A c(2);
   A d=getA();
  // A b(d);
  // printf("a.a:%d\nb.a%d\n",a.a,b.a);
   return 0;
}
