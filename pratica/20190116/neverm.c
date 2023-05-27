#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

void handler(int fd, int wd, char *dirname){
    size_t len;
    char buf[4096];
    struct inotify_event *event;
    len = read(fd, buf, sizeof(buf));
    event = (struct inotify_event *) buf;
    char *dirOK;
    if(dirname[strlen(dirname)-1]=='/')
        dirOK = dirname;
    else{
        dirOK = malloc(sizeof(char)*strlen(dirname)+2);
        strcpy(dirOK, dirname);
        strcat(dirOK, "/");
    }
    if(event->mask & IN_DELETE){
        char *filepath = malloc(sizeof(char)*(strlen(event->name)+strlen(dirOK)+1));
        fclose(fopen(strcat(strcpy(filepath, dirOK), event->name), "w+"));
    }
}
int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, argv[1], IN_ONLYDIR|IN_DELETE);
    if(wd==-1)
        exit(EXIT_FAILURE);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    int poll_num;
    for(;;){
        poll_num = poll(&pfd, 1, 0);
        if(poll_num>0 && pfd.revents & POLLIN > 0)
            handler(fd, wd, argv[1]);
        pfd.revents = 0;
        
    }
    return 0;
}