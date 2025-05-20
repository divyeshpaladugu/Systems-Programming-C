#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s)
{
    perror(s);
    exit(1);
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(char *s1, char *s2)
{
    // TODO 
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int lenght = l1 + l2 + 1;
    char* ptr;
    ptr = (char*)malloc(lenght * sizeof(char));

    strcpy(ptr, s1);
    strcat(ptr, s2);

    return ptr;
}

int main(int argc, char *argv[])
{
    char    *s;

    s = my_strcat("", argv[0]);

    for (int i = 1; i < argc; i ++) 
    {
        char *temp = my_strcat(s, argv[i]);
        free(s);
        s = temp;
    }

    printf("%s\n", s);
    free(s);
    return 0;
}