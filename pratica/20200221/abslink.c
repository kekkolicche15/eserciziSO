#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    struct stat buf;
    lstat(argv[1], &buf);
    if((buf.st_mode & S_IFMT) == S_IFLNK){
        char *real = malloc(1024);
        realpath(argv[1], real);
        unlink(argv[1]);
        symlink(real, argv[1]);
    }
    exit(EXIT_SUCCESS);
}