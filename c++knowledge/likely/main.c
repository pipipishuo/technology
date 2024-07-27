#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

int main(char *argv[], int argc)
{
   int a;

   /* Get the value from somewhere GCC can't optimize */
   a = 2;
  
   if (unlikely(a==2))
      a++;
   else
      a--;

   printf ("%d\n", a);

   return 0;
}
