#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

int main(int argc, char*argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    DIR *dr = opendir(argv[1]);
    struct dirent *de;
    while(de=readdir(dr)){
            char *real = malloc(1024);
            realpath(de->d_name, real);
            printf("%s\n", real);
        }
    exit(EXIT_SUCCESS);
}