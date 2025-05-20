#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    pid_t child;
    int exitStatus;

    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // TODO
    child = fork();
    if (child < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }

    if (child == 0)
    {
        if (execlp(argv[1], argv[1], argv[2], NULL) == -1)
        {
            perror("execlp()");
            exit(EXIT_FAILURE);
        }
    }



    return 0;
}