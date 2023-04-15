#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>

/* int main(int argc, char *argv[]){
    int status;
    _Bool relaunch = 1;
    do{
        switch(fork()){
            case -1:
                exit(0);
            case 0:
                execvp(argv[1], argv+1);
                _exit(0);
                break;
            default:
                wait(&status);
                if(!WIFEXITED(status) || WEXITSTATUS(status)!=0)
                    relaunch = 0;
                break;
        }
    }while(relaunch);
} */

int main(int argc, char *argv[]){
    int status;
    _Bool relaunch = 1;
    struct timeval getusec;
    suseconds_t start, stop;
    gettimeofday(&getusec, NULL);
    start = getusec.tv_usec;
    do{
        switch(fork()){
            case -1:
                exit(0);
            case 0:
                execvp(argv[1], argv+1);
                _exit(0);
                break;
            default:
                wait(&status);
                gettimeofday(&getusec, NULL);
                stop = getusec.tv_usec;
                if(!WIFEXITED(status) || WEXITSTATUS(status)!=0 || stop - start < 1000)
                    relaunch = 0;
                else
                    start = stop;
                break;
        }
    }while(relaunch);
}