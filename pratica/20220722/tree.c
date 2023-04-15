#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void tree();
int main(int argc, char *argv[]){
    if(argc<2)
        exit(1);
    tree(open(argv[1],O_RDONLY), 0);

}
void tree(int dfd, int nosp){
    if(dfd!=-1){
      DIR *d;
      if((d=fdopendir(dfd))!=NULL){
        struct dirent *dp;
        struct stat sb;
        while((dp = readdir(d))!=NULL){
            if(dp->d_name[0]!='.' || dp->d_name[1]!='.' && dp->d_name[1]!='\0'){ // ignore . and .. but show hidden files/dirs
            //if(dp->d_name[0]!='.'){ // ignore ., .., and hidden files 
            	printf("%*s%s\n", nosp*4, "",dp->d_name);//%*s , "" are for indentation
              fstatat(dfd, dp->d_name, &sb, 0);
              if(S_ISDIR(sb.st_mode)) //it's a dir...
              	tree(openat(dfd, dp->d_name, O_RDONLY), nosp+1);
            }
        }
      }
    }
}