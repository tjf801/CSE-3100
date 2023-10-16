#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>

double uniform_random(){
  return drand48();
}

int count_n(){
  double sum = 0;
  double rand;
  int n = 0;
  while(sum <= 1){
    rand = uniform_random();
    sum += rand;
    n++;
  }
  return n;
}

int main()
{
  srand48(1551142124);
  int n = 2000000;
  unsigned int total = 0;
  for(int i = 0; i < n; i++)
    total += count_n();
  double e = 1.0*total/n;
 
  printf("e: %.3f\n", e);
  return 0;
}
