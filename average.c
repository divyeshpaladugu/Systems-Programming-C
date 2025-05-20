#include <stdio.h>

int main(void)
{

double x;
double total;
double average;
int i = 1;

while (scanf("%lf", &x) == 1) { // pay attention to %lf
total += x;
average = total/i;
i++;

printf("Total=%f Average=%f\n", total, average); // pay attention to %f
};

return 0;
}