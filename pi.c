#include <stdio.h>

int main()
{
    int n, i;

    printf("n = ");
    scanf("%d", &n);

    double pi = 0.0;
    double sum = 0.0;
    double factor = 1.0;

    for (i = 0; i <= n; i++)
    {
        double num = ( (4.0 / (8 * i + 1)) - (2.0 / (8 * i + 4)) 
                      - (1.0 / (8 * i + 5)) - (1.0 / (8 * i + 6)) ) / factor;
        sum += num;
        factor *= 16.0;
    }

    pi += sum;

    printf("PI = %.10f\n", pi);
    return 0;
}