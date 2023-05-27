#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define EVENT_TARGET IN_CREATE|IN_MOVED_TO

void name2vect(char *src, char *dest[], char sep){
    int destMIdx = 0;
    int destIdx = 0;
    int srcIdx = 0;
    do{
        if(src[srcIdx]==sep){
            dest[destMIdx][destIdx]='\0';
            destMIdx++;
            destIdx=0;
        }
        else{
            dest[destMIdx][destIdx++] = src[srcIdx];
        }
        srcIdx++;
    }while(src[srcIdx]!=0);
    dest[destMIdx++][++destIdx] = 0;
    dest[destMIdx] = NULL;
}

void handler(int fd, int wd, char *dirname){
    size_t len;
    char buf[4096];
    struct inotify_event *event;
    len = read(fd, buf, sizeof(buf));
    event = (struct inotify_event *) buf;
    if(event->mask & EVENT_TARGET){
        char *cmd[1024];
        for(int i=0; i<1024; i++)
            cmd[i] = (char*)calloc(1024, sizeof(char));
        name2vect(event->name, cmd, ' ');
        char *filepath = malloc(strlen(dirname)+strlen(event->name)+1);
        strcpy(filepath, dirname);
        strcat(filepath, event->name);
        unlink(filepath);
        if(fork()==0)
            execvp(cmd[0], cmd);
        else
            wait(NULL);
    }
}
int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    char *dirname;
    DIR *d;
    if(argv[1][strlen(argv[1])-1]=='/'){
        dirname = malloc(strlen(argv[1]+1));
        strcpy(dirname, argv[1]);
    }
    else{
        dirname = malloc(strlen(argv[1])+2);
        strcpy(dirname, argv[1]);
        strcat(dirname, "/");
    }
    if((d=opendir(argv[1]))==NULL)
        exit(EXIT_FAILURE);
    readdir(d), readdir(d);
    if(readdir(d)!=NULL)
        exit(EXIT_FAILURE);
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, dirname, IN_ONLYDIR|EVENT_TARGET);
    if(wd==-1)
        exit(EXIT_FAILURE);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    int poll_num;
    for(;;){
        poll_num = poll(&pfd, 1, 0);
        if(poll_num>0 && pfd.revents & POLLIN > 0)
            handler(fd, wd, dirname);
        pfd.revents = 0;
        
    }
    return 0;
}