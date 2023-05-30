#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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
    if(argc<3){
        printf("usage: ./pcmp <file1> <file2>");
        exit(EXIT_FAILURE);
    }
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    
    fseek(f1, 0, SEEK_END);
    int f1size = ftell(f1);

    fseek(f2, 0, SEEK_END);
    int f2size = ftell(f2);

    fclose(f1);
    fclose(f2);

    if(f1size!=f2size)
        printf("%s %s differ\n", argv[1], argv[2]);
    else{
        pid_t pid[2];
        if((pid[0]=fork())){
            if((pid[1]=fork())){
                int status;
                pid_t term = wait(&status);
                int res = WEXITSTATUS(status);
                if(WEXITSTATUS(status)!=1){
                    printf("%s %s differ\n", argv[1], argv[2]);
                    kill(pid[0], SIGTERM);
                    kill(pid[1], SIGTERM);
                }
                wait(&status);
                if(WEXITSTATUS(status)!=1)
                    printf("%s %s differ\n", argv[1], argv[2]);
                
            }
            else _exit(compare(argv[1], argv[2], f1size/2, f1size));
        }
        else _exit(compare(argv[1], argv[2], 0, f1size/2));
    }
    exit(EXIT_SUCCESS);
}
