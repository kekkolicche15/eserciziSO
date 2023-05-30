#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int                fd;
    ssize_t            s;
    uint64_t           exp, tot_exp, max_exp;
    struct timespec    now;
    struct itimerspec  new_value;

    if (argc != 2 && argc != 4) {
        fprintf(stderr, "%s init-secs [interval-secs max-exp]\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    if (clock_gettime(CLOCK_REALTIME, &now) == -1)
        err(EXIT_FAILURE, "clock_gettime");

    /* Create a CLOCK_REALTIME absolute timer with initial
        expiration and interval as specified in command line. */

    long nsec = atol(argv[1])/1000;
    long nnsec = atol(argv[1])*1000000%1000000000;

    new_value.it_value.tv_sec = now.tv_sec + nsec;
    new_value.it_value.tv_nsec = now.tv_nsec + 1;
    if (argc == 2) {
        new_value.it_interval.tv_sec = 0;
        max_exp = 1;
    } else {
        new_value.it_interval.tv_sec = atoi(argv[2]);
        max_exp = atoi(argv[3]);
    }
    new_value.it_interval.tv_nsec = 0;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        err(EXIT_FAILURE, "timerfd_create");

    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
        err(EXIT_FAILURE, "timerfd_settime");

    printf("timer started\n");

    for (tot_exp = 0; tot_exp < max_exp;) {
        s = read(fd, &exp, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            err(EXIT_FAILURE, "read");

        tot_exp += exp;
        printf("read: %" PRIu64 "; total=%" PRIu64 "\n", exp, tot_exp);
    }

    exit(EXIT_SUCCESS);
}