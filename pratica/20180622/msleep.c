//21:20

#include <sys/timerfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <poll.h>


int main(int argc, char*argv[]){
    if(argc<2){
        printf("usage: ./msleep <msec>");
        exit(EXIT_FAILURE);
    }
    int timerfd = timerfd_create(CLOCK_BOOTTIME, 0);
    struct itimerspec new_value = { { 0, 0 } , { 0, 1 } };


    timerfd_settime(timerfd, 0, &new_value, NULL);


        /* Wait */
        fd_set rfds;
        int retval;

        /* Watch timefd file descriptor */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(timerfd, &rfds);

    select(timerfd+1, &rfds, NULL, NULL, NULL);
}

//todo vedere implementazione file descriptor 