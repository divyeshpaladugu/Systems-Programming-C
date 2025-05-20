#include <stdio.h>
#include <stdlib.h>

int sum_of_squares(int num) {
    int sum = 0;
    while (num > 0) {
        int digit = num % 10;
        sum += digit * digit;
        num /= 10;
    }
    return sum;
}

int main() {
    int n;
    printf("n = ");
    scanf("%d", &n);
    int m = n;

    while (n != 1 && n != 4) {
        n = sum_of_squares(n);
        printf("%d\n", n);
    }

    if (n == 1)
        printf("%d is a happy number.\n", m);
    else
        printf("%d is NOT a happy number.\n", m);

    return 0;
}
