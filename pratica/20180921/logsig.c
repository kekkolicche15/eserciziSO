#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <unistd.h>
#include <time.h>

#define MAX_PID_DIGITS 7

int main(void){
    int                      sfd;
    ssize_t                  s;
    sigset_t                 mask;
    struct signalfd_siginfo  fdsi;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    printf("%d\n", getpid());
    time_t t = time(NULL);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
        err(EXIT_FAILURE, "sigprocmask");

    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1)
        err(EXIT_FAILURE, "signalfd");

    for (;;) {
        s = read(sfd, &fdsi, sizeof(fdsi));
        if (s != sizeof(fdsi))
            err(EXIT_FAILURE, "read");
        if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2) {
            struct tm *tm = localtime(&t); //sctime(tm) --> str;
            //Fri Sep 21 10:40:56 CEST 2018
            //%a %b %y %H:%M:%S %Z %Y
            //3 3 2 2:2:2 4 4
            char *str = malloc(sizeof(char)*30);
            strftime(str, 30, "%a %b %y %H:%M:%S %Z %Y", tm);

            char *filename = malloc(sizeof(char)*(MAX_PID_DIGITS+1));
            snprintf(filename, MAX_PID_DIGITS+1, "%d", fdsi.ssi_pid);
            FILE *fout = fopen(filename, "a+");
            fprintf(fout, "USR%d %s\n", fdsi.ssi_signo == SIGUSR1?1:2, str);
            fclose(fout);
        } else
            printf("Read unexpected signal\n");
    }
}