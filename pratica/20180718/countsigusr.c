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

    int val = 0;

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
        if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2)
            printf("SIGUSR1/2 received; new val: %d\n", fdsi.ssi_signo==SIGUSR1?++val:--val);
    }
}