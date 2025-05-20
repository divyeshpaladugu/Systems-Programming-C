#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) 
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }
    
    // child runs first cmd
    pid_t cmd1_runner_pid = fork();
    if (cmd1_runner_pid == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    
    if (cmd1_runner_pid == 0) {
        // in the child process
        //executes the first cmd
        if (execlp(argv[1], argv[1], argv[2], (char *)NULL) == -1) {
            perror("execlp()");
            exit(EXIT_FAILURE);
        }
    }
    
    //parent process
    int status_1;
    pid_t err1 = waitpid(cmd1_runner_pid, &status_1, 0);
    if (err1 == -1) {
        perror("waitpid()");
        exit(EXIT_FAILURE);
    }
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status_1), WEXITSTATUS(status_1));
    
    // child second command
    pid_t cmd2_runner_pid = fork();
    if (cmd2_runner_pid == -1) {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    
    if (cmd2_runner_pid == 0) {
        //run second command
        if (execvp(argv[3], &argv[3]) == -1) {
            perror("execvp()");
            exit(EXIT_FAILURE);
        }
    }
    
    // wait for 2nd cmd completion
    int status_2;
    pid_t err2 = waitpid(cmd2_runner_pid, &status_2, 0);
    if (err2 == -1) {
        perror("waitpid()");
        exit(EXIT_FAILURE);
    }
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status_2), WEXITSTATUS(status_2));
    
    return 0;
}
