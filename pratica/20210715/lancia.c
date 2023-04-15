#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    char *resolved = malloc(1024);
    realpath(argv[1], resolved);
    void *handle = dlopen(resolved, RTLD_LAZY);
    int (*hw)(int, char**);
    hw = (int(*)(int, char**))dlsym(handle, "main");
    hw(argc-1, &argv[1]);
    return 0;
}