#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    DIR *d;
    if((d = opendir(argv[1]))==NULL)
        exit(EXIT_FAILURE);
    struct dirent *de;
    struct stat sb;
    char *fullpath, *interm, *fullinterm;
    while((de=readdir(d))!=NULL)
        if(de->d_type==10){
            interm = calloc(1024, 1);
            fullinterm = calloc(1024, 1);
            fullpath = calloc(1024, 1);
            fullpath = strcat(strcat(strcat(fullpath, argv[1]), "/"), de->d_name);
            readlink(fullpath, interm, sizeof(interm)-1);
            fullinterm = strcat(strcat(strcat(fullinterm, argv[1]), "/"), interm);
            lstat(fullinterm, &sb);
            printf("%s, %s\n", fullpath, fullinterm);
            if((sb.st_mode & S_IFMT) == S_IFLNK){
                char *realfile = calloc(1024, 1);
                realpath(fullpath, realfile);
                unlink(fullpath);
                link(realfile, fullpath);
            }
        }
    return 0;
}