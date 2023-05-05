#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

void processOptions(char options[], int type){
    char *opt;
    opt = strtok(options, " ");

    while(opt != NULL ) {
      printf( " %s\n", opt);

        
    
      opt = strtok(NULL, " ");
   }


}

int main(int argc, char* argv[]){
    if(argc < 2){
        perror("Invalid number of arguments!");
        exit(0);
    }

    for(int i = 1; i < argc; i++){
        struct stat file;

        if(lstat(argv[i], &file)){
            perror("Error!\n");
            exit(2);
        }

        char options[100];

        if(S_ISDIR(file.st_mode)){
            printf("-n show name\n-d show size\n-a show access rights\n-c show total number of files with .c extension\n");
            fgets(options, sizeof(options), stdin);
            processOptionsDir(options);
        }
        if(S_ISLNK(file.st_mode)){
            printf("-n show name\n-l delete symbolic links\n-d show size of sybolic link\n-t show size of target file\n-a show access rights\n");
        }
        if(S_ISREG(file.st_mode)){
            printf("-n show name\n-d show size\n-h show the hard link count\n-m show time of last modification\n-a show access rights\n-l create a symbolic link\n");
        }

    }
}