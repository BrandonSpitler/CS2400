#include <stdio.h>
#include <math.h>

#define rdtscll(val) \
     __asm__ __volatile__ ("rdtsc" : "=A" (val))
int x;
int y;

int main()
{
  int x;
  long long foo;
  long long bar;
  fprintf(stdout, "sizeof(foo) is %d\n", sizeof(foo));

  rdtscll(foo);
  fprintf(stdout, "foo is %lld\n", foo);

  rdtscll(foo);
  //  x++;
  rdtscll(bar);

  fprintf(stdout, "foo is %lld\n", foo);
  fprintf(stdout, "bar is %lld\n", bar);
  long long diff = bar - foo;
  fprintf(stdout, "difference is %lld\n", diff);
}
