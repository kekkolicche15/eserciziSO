#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

char *strptime();


int main(int argc, char*argv[]){
    if(argc<2)
        exit(EXIT_FAILURE);
    FILE *fin = fopen(argv[1], "r");
    char *line = NULL;
    size_t len = 0;
    //Sep 20 19:58:32 
    //%b %d %T
    struct tm DR_time = {};
    int old_time = -1;
    while(getline(&line, &len, fin)!=-1){
        strptime(line, "%b %d %T", &DR_time);
        if(old_time==-1)
            old_time = mktime(&DR_time);
        else{
            sleep(mktime(&DR_time)-old_time);
            old_time = mktime(&DR_time);
        }
        printf("%s", line);
    }
    exit(EXIT_SUCCESS);
}