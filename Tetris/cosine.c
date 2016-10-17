#include <stdio.h>
#include <math.h>

double mycosine(double);
int exponential(int);

int main()
{
  printf("%d\n", exponential(5));
}

double mycosine(double x)
{
  double total= 1, term = 2, num = 0;
  while (term >= 0.001){
    term = (-1*pow(x, num)) / exponential(num);
    total = total + term;
    term = (term + 2) * -1;
  }
  return total;
}

int exponential(int y)
{
  int i, total = 1;
  for (i = y; i > 0; i--){
    total = total * i;
  }
  return total;
}
