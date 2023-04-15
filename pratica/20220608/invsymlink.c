#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

void invert(char *symlnk){
    char real[1000];
    realpath(symlnk, real);
    unlink(symlnk);
    FILE* symfile = fopen(symlnk, "w+");
    FILE* realfile = fopen(real, "r");
    char c;
    while((c=fgetc(realfile))!=EOF)
        fputc(c, symfile);
    fclose(symfile);
    fclose(realfile);
    unlink(real);
    symlink(symlnk, real);
    printf("Scambiati %s e %s\n", symlnk, real);
}

int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    struct stat buf;
    lstat(argv[1], &buf);
    if(S_ISLNK(buf.st_mode))
        invert(argv[1]);
    else if(S_ISDIR(buf.st_mode)){
        DIR* dr = opendir(argv[1]);
        struct dirent* de;
        while((de=readdir(dr)))
            if(de->d_type==10)//symlink
                invert(de->d_name);
    }
    return 0;
}