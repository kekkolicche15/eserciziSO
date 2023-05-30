#include <dirent.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

char *joinpath(char *base, char *file){
    char *res = calloc(strlen(base)+strlen(file)+2, sizeof(char));
    strcpy(res, base);
    if(base[strlen(base)-1]!='/')
        strcat(res, "/");
    return strcat(res, file);
}


void changefilemt(char *path){
    struct stat attr;
    stat(path, &attr);
    struct timeval times[2];
    times[1].tv_sec = attr.st_mtime-864000;
    utimes(path, times);
}

int main(int argc, char *argv[]){
    if(argc>1)
        for(int i=1; i<argc; i++)
            changefilemt(argv[i]);
    else{
        DIR *d = opendir(".");
        struct dirent *de;
        while((de=readdir(d))!=NULL){
            changefilemt(joinpath(".", de->d_name));
        }
    }
}