#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define NONE 0
#define COPY 1
#define NOSYM 2
#define NOABS 3

void check(char *fname, int inode, char *pname, int opt){
    DIR *dr = opendir(pname);
    struct dirent *de;
    while((de=readdir(dr))){
        if(de->d_name[0]!='.'){
            if(de->d_type==4){
                char *newdir = malloc(1024);
                strcpy(newdir, pname);
                strcat(strcat(newdir, "/"), de->d_name);
                check(fname, inode, newdir, opt);
            }
            else if(de->d_type==10){
                char *lnk = malloc(1024);
                char *full = malloc(1024);
                strcpy(full, pname);
                strcat(strcat(full, "/"), de->d_name);
                realpath(full, lnk);
                if(!strcmp(lnk, fname))
                    if(opt==NOSYM){
                        unlink(full);
                        link(fname, full);
                        printf("nuovo hardlink in %s\n", full);
                    }
                    else
                        printf("symlink %s\n", full);
            }
            else if(de->d_type==8){
                char *full = malloc(1024);
                strcpy(full, pname);
                strcat(strcat(full, "/"), de->d_name);
                char *rl = malloc(1024);
                realpath(full, rl);
                if(strcmp(fname, rl)){
                    struct stat buf;
                    stat(rl, &buf);
                    if(inode==buf.st_ino){
                        if(opt==COPY){
                            unlink(full);
                            int c;
                            FILE *in = fopen(fname, "r");
                            FILE *out = fopen(full, "w+");
                            while((c=getc(in))!=EOF)
                                putc(c, out);
                            printf("copiato in %s\n", full);
                        }
                        else if(opt==NOABS){
                            unlink(full);
                            symlink(fname, full);
                            printf("nuovo symlink in %s\n", full);
                        }
                        else
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
    int c, opt=0;
    if((c=getopt(argc, argv, "cls"))!=-1){
        if(c=='c')
            opt=COPY;
        else if(c=='l')
            opt=NOSYM;
        else
            opt=NOABS;
    }
    check(fname, buf.st_ino, argv[2], opt);
    exit(EXIT_SUCCESS);
}