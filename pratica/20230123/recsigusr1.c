#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void alertsig(int signo){
    printf("Received SIGUSR1 (%d)\n", signo);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    if(argc<2){
        printf("usage: %s <pipe>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *fout;
    if((fout = fopen(argv[1], "w"))==NULL){
        printf("error with pipe\n");
        exit(EXIT_FAILURE);
    }
    struct sigaction sa;
    sa.sa_handler = alertsig;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    fprintf(fout, "%d %d\n", getpid(), SIGUSR1);
    fclose(fout);
    while(1)
        sleep(20);
}