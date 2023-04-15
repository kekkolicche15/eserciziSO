#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void received_string(int signo, siginfo_t *info, void *context){
    long msg_long = (long)info->si_value.sival_ptr;
    if(msg_long == 0)
        printf("\n");
    else{
        char msg;
        for(int i=0, last = 0; i<8; i++){
            last = msg_long%256;
            msg_long = msg_long>>8;
            printf("%c", last);
        }
    }
    union sigval v;
    sigqueue(info->si_pid, SIGUSR1, v);
}

int main(int argc, char *argv[]){
    printf("%d\n", getpid());
    struct sigaction act;
    act.sa_sigaction = received_string;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    while(1)
        sleep(20);
    return 0;
}