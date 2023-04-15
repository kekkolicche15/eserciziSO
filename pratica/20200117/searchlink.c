#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void check(char *fname, int inode, char *pname){
    DIR *dr = opendir(pname);
    struct dirent *de;
    while((de=readdir(dr))){
        if(de->d_name[0]!='.'){
            if(de->d_type==4){
                char *newdir = malloc(1024);
                strcpy(newdir, pname);
                strcat(strcat(newdir, "/"), de->d_name);
                check(fname, inode, newdir);
            }
            else if(de->d_type==10){ //symlink
                char *lnk = malloc(1024);
                char *full = malloc(1024);
                strcpy(full, pname);
                strcat(strcat(full, "/"), de->d_name);
                realpath(full, lnk);
                if(!strcmp(lnk, fname))
                    printf("symlink %s\n", full);
            }
            else if(de->d_type==8){ //file
                char *full = malloc(1024);
                strcpy(full, pname);
                strcat(strcat(full, "/"), de->d_name);
                char *rl = malloc(1024);
                realpath(full, rl);
                if(strcmp(fname, rl)){
                    struct stat buf;
                    stat(rl, &buf);
                    if(inode==buf.st_ino){   
                        printf("link %s\n", full);
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    if(argc<3)
        exit(EXIT_FAILURE);
    char *fname = malloc(1024);
    realpath(argv[1], fname);
    struct stat buf;
    stat(fname, &buf);
    check(fname, buf.st_ino, argv[2]);
    exit(EXIT_SUCCESS);
}