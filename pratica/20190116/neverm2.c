#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

char* backupdir;

char *fixdirname(char *old){
    char *new;
    if(old[strlen(old)-1]=='/'){
        new = malloc(sizeof(char)*strlen(old)+1);
        strcpy(new, old);
    }
    else{
        new = malloc(sizeof(char)*strlen(old)+2);
        strcpy(new, old);
        strcat(new, "/");
    }
    return new;
}

char *addprefix(char *prefix, char *str){
    char *res = malloc(sizeof(char)*(strlen(prefix)+strlen(str)+1));
    return strcat(strcpy(res, prefix), str);
}

char *getfreedir(){
    DIR *tmp = opendir(".");
    char *name = malloc(sizeof(char)*13);
    strcpy(name, "./");
    name[12] = 0;
    do{
        for(int i=2; i<12; i++)
            name[i] = random() % 10+48;
        mkdir(name, 0700);
    }while(errno==EEXIST);
    return name;
}

void addbkp(char *src_dir, char *dest_dir, char *name){
    char *src = addprefix(src_dir, name);
    char *dest = addprefix(dest_dir, name);
    link(src,dest);
    free(src);
    free(dest);
}

void setbkp(char *dir, char *backupdir){
    char *safedir = fixdirname(dir);
    char *safebkp = fixdirname(backupdir);
    DIR *d = opendir(dir);
    struct dirent *de;
    while((de=readdir(d))!=NULL)
        addbkp(safedir, safebkp, de->d_name);
    free(safedir);
    free(safebkp);
    closedir(d);
}

void nosigint(int signum){
    DIR *d = opendir(backupdir);
    struct dirent *de;
    while((de=readdir(d))!=NULL){
        char *safebkp = addprefix(fixdirname(backupdir), de->d_name);
        unlink(safebkp);
        free(safebkp);
    }
    rmdir(backupdir);
    exit(EXIT_SUCCESS);
}

void handler(int fd, int wd, char *dir){
    size_t len;
    char buf[4096];
    struct inotify_event *event;
    len = read(fd, buf, sizeof(buf));
    event = (struct inotify_event *) buf;
    char *safesrcdir = fixdirname(dir);
    char *safebkpdir = fixdirname(backupdir);
    if(event->mask & IN_CREATE)
        addbkp(safesrcdir, safebkpdir, event->name);
    else if(event->mask & IN_DELETE){
        char *safesrcfile = addprefix(safesrcdir, event->name);
        char *safebkpfile = addprefix(safebkpdir, event->name);
        link(safebkpfile, safesrcfile);
        free(safesrcfile);
        free(safebkpfile);
    }
    free(safesrcdir);
    free(safebkpdir);
}
int main(int argc, char *argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    struct sigaction sa;
    sa.sa_handler = nosigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    srandom(time(NULL));
    backupdir = getfreedir();
    setbkp(argv[1], backupdir);

    int fd = inotify_init();
    int wd = inotify_add_watch(fd, argv[1], IN_ONLYDIR|IN_DELETE|IN_CREATE);
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
}