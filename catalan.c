#include <stdio.h>
#include <stdlib.h>

/* This function calculates the n-th Catalan number.
 * n is assumed to be non-negative.
 * Do not worry about overflow.
 * Do not print anything in this function.
 * Do not use floating-point numbers (or operations).
 */
long catalan_number(int n)
{
    if (n < 0)
        return 0;

    if (n == 0)
        return 1;

    long *catalan = (long *)malloc((n + 1) * sizeof(long));

    // base case
    catalan[0] = 1;

    for (int i = 1; i <= n; i++) {
        catalan[i] = 0;
        for (int j = 0; j < i; j++) {
            catalan[i] += catalan[j] * catalan[i - j - 1];
        }
    }

    long result = catalan[n];
    free(catalan);
    return result;
}

/* do not change main function */
int main(void)
{
    int n;

    while (scanf("%d", &n) == 1) {
        if (n < 0)
            printf("C(%2d) is not defined.\n", n);
        else
            printf("C(%2d)=%18ld\n", n, catalan_number(n));
    }
    return 0;
}
