#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <getopt.h>

#define DEF_VALUE 2

int compare(char *filename1, char *filename2, int start, int end){
    FILE *f = fopen(filename1, "r");
    fseek(f, start, SEEK_SET);
    FILE *g = fopen(filename2, "r");
    fseek(g, start, SEEK_SET);

    for(int i=start; i<end; i++)
        if(getc(f)!=getc(g))
            return 0;
    return 1;
}

int main(int argc, char *argv[]){
    int opt, n;
    _Bool f = 0;
    while ((opt = getopt(argc, argv, "j:")) != -1) {
        switch(opt){
        case 'j':
            n = atoi(optarg);
            f=1;
        }
    }
    char *fname1 = argv[f?3:1];
    char *fname2 = argv[f?4:2];
    n = n>0?n:DEF_VALUE;
    FILE *f1 = fopen(fname1, "r");
    FILE *f2 = fopen(fname2, "r");
    fseek(f1, 0, SEEK_END);
    int f1size = ftell(f1);
    fseek(f2, 0, SEEK_END);
    int f2size = ftell(f2);

    fclose(f1);
    fclose(f2);

    if(f1size!=f2size)
        printf("%s %s differ\n", fname1, fname2);
    else{
        pid_t pid[n];
        for(int i=0; i<n; i++)
            if(!(pid[i]=fork()))
                _exit(compare(fname1, fname2, f1size*i/n, f1size*(i+1)/n));

        for(int i=0; i<n; i++){
            int status;
            pid_t rec = wait(&status);
            if(!WIFEXITED(status)){
                printf("%d: Error in _exit\n", rec);
                exit(EXIT_FAILURE);
            }
            if(!WEXITSTATUS(status)){
                printf("%s %s differ\n", fname1, fname2);
                for(int j=0; j<n; j++)
                    kill(pid[j], SIGTERM);
            }
        }
    }
    exit(EXIT_SUCCESS);
}