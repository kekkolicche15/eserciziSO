#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>

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

void handler(int fd, int wd){
    size_t len;
    char buf[4096];
    struct inotify_event *event;
    len = read(fd, buf, sizeof(buf));
    event = (struct inotify_event *) buf;
    if(event->mask & IN_CREATE){
        char *cmd[1024];
        for(int i=0; i<1024; i++)
            cmd[i] = (char*)calloc(1024, sizeof(char));
        name2vect(event->name, cmd, ' ');
        char *filepath = malloc(6+strlen(event->name));
        strcpy(filepath, "exec/");
        strcat(filepath, event->name);
        if(fork()==0){
            int fd[2];
            pipe(fd);
            FILE *outpt = fopen(filepath, "w");
            dup2(fileno(outpt), STDOUT_FILENO);
            execvp(cmd[0], cmd);
            fclose(outpt);
        }
    }
}
int main(int argc, char *argv[]){
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, "./exec", IN_ONLYDIR|IN_CREATE);
    if(wd==-1)
        exit(EXIT_FAILURE);
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    int poll_num;
    for(;;){
        poll_num = poll(&pfd, 1, 0);
        if(poll_num>0 && pfd.revents & POLLIN > 0)
            handler(fd, wd);
        pfd.revents = 0;
        
    }
    return 0;
}