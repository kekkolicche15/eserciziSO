#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <getopt.h>

#define DEF_VALUE 2


void copychunk(char *filename1, char *filename2, int start, int end){
    FILE *f = fopen(filename1, "r");
    fseek(f, start, SEEK_SET);
    FILE *g = fopen(filename2, "r");
    fseek(g, start, SEEK_SET);
    for(int i=start; i<end; i++)
        putc(getc(f), g);
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
    fclose(f1);

    pid_t pid[n];
    for(int i=0; i<n; i++)
        if(!(pid[i]=fork()))
            _exit(compare(fname1, fname2, f1size*i/n, f1size*(i+1)/n));

    for(int i=0; i<n; i++){
        int status;
        wait(&status);
    }
    exit(EXIT_SUCCESS);
}