#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    prctl(PR_SET_CHILD_SUBREAPER, 1);
    pid_t p;
    if(!(p=fork()))
        if(!(fork())){
            prctl(PR_SET_NAME, "nipote");
            // sleep(10);
            printf("NIPOTE: getppid(): %d\n", getppid());
            for(volatile int i=0; i<1<<20;i++);
            printf("NIPOTE: getppid(): %d\n", getppid());
            _exit(0);
        }
        else{
            prctl(PR_SET_NAME, "figlio");
            // sleep(10);
            printf("FIGLIO: getpid(): %d\n", getpid());
            _exit(0);
        }
    else{
        printf("PADRE: getpid(): %d\n", getpid());
        prctl(PR_SET_NAME, "padre");
        waitpid(p, NULL, WUNTRACED);
        printf("figlio morto\n");
        wait(NULL);
        printf("nipote morto\n");
    }
}