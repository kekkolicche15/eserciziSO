#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <execs.h>

int main(){
    char *cmd1, *cmd2;
    size_t len1, len2;
    int res;
    getline(&cmd1, &len1, stdin);
    getline(&cmd2, &len2, stdin);

    int pipefd[2];
    pipe(pipefd);

    if(fork()){
        wait(NULL);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        system_nosh(cmd2);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else{
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        system_nosh(cmd1);
        exit(EXIT_SUCCESS);
    }
}