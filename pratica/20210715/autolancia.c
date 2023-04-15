#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    char *resolved = malloc(1024);
    realpath(argv[1], resolved);
    void *handle;
    if(handle=dlopen(resolved, RTLD_LAZY)){
        int (*hw)(int, char**);
        hw = (int(*)(int, char**))dlsym(handle, "main");
        hw(argc-1, &argv[1]);
    }
    else{
        char *cmd = malloc(3+sizeof(argv[1]));
        strcpy(cmd, "./");
        strcat(cmd, argv[1]);
        execvp(cmd, &argv[1]);
    }
    return 0;
}