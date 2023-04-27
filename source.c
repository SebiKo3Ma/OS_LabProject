#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc < 2){
        perror("Invalid number of arguments!");
        exit(0);
    }

    for(int i = 1; i < argc; i++){
        FILE *f;
        struct stat file;

        if((f = open(argv[i], O_WRONLY)) <0 ){
            perror("Cannot open file!\n");
            exit(1);
        }

        if(fstat(f, &file)){
            perror("Error!\n");
            exit(2);
        }

        if(S_ISREG(file.st_mode) && !(S_ISLNK(file.st_mode))){
            printf("-n show name\n-d show size\n-h show the hard link count\n-m show time of last modification\n-a show access rights\n-l create a symbolic link\n");
        }
        else if(S_ISDIR(file.st_mode)){
            printf("-n show name\n-d show size\n-a show access rights\n-c show total number of files with .c extension\n");
        }
        else if(S_ISLNK(file.st_mode)){
            printf("-n show name\n-l delete symbolic links\n-d show size of sybolic link\n-t show size of target file\n-a show access rights\n");
        }

    }


}