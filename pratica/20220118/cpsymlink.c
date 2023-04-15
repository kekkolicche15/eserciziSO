#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    DIR *dr = opendir(argv[1]);
    struct dirent *de;
    char real[100];
    while(de=readdir(dr)){
        if(de->d_type==10){
            char fullpath[strlen(de->d_name)+strlen(argv[1])+2];
            snprintf(fullpath, sizeof fullpath, "%s/%s", argv[1], de->d_name);
            realpath(fullpath, real);
            unlink(fullpath);
            link(fullpath, real);
        };
    }
    return 0;
} */

int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    DIR *dr = opendir(argv[1]);
    struct dirent *de;
    char real[100];
    while(de=readdir(dr)){
        if(de->d_type==10){
            char fullpath[strlen(de->d_name)+strlen(argv[1])+2];
            snprintf(fullpath, sizeof fullpath, "%s/%s", argv[1], de->d_name);
            realpath(fullpath, real);
            unlink(fullpath);
            FILE *source = fopen(real, "r");
            FILE *dest = fopen(fullpath, "w+");
            short buf;
            while(buf=getc(source))
                putc(buf, dest);
            fopen(source);
            fopen(dest);
        };
    }
    return 0;
}