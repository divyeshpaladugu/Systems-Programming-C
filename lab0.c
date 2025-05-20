#include <stdio.h>
int main(void)
{
printf("Hello, World!\n");

int i = 0;
int x = 0;
while (i < 200) {
    x += i;
    i += 2;
}
printf("%d\n", x);

return 0;
}