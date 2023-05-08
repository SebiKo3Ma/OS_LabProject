#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

void processOptionsDir(char options[]){
    char *opt;
    opt = strtok(options, " ");

    while(opt != NULL ) {
        if((strlen(opt) == 2 && opt[0] == '-')){
            switch(opt[1]){
                case 'n':
                    printf( "n %s\n", opt);
                break;

                case 'd':
                    printf( "d %s\n", opt);
                break;

                case 'a':
                    printf( "a %s\n", opt);
                break;

                case 'c':
                    printf( "c %s\n", opt);
                break;

                default:
                    printf("Invalid option: %s\n", opt);
            }
        } else printf("Invalid option: %s\n", opt);
        
      opt = strtok(NULL, " ");
   }
}


void processOptionsFile(char options[]){
    char *opt;
    opt = strtok(options, " ");

    while(opt != NULL ) {
        if((strlen(opt) == 2 && opt[0] == '-')){
            switch(opt[1]){
                case 'n':
                    printf( "n %s\n", opt);
                break;

                case 'd':
                    printf( "d %s\n", opt);
                break;

                case 'h':
                    printf( "h %s\n", opt);
                break;

                case 'm':
                    printf( "m %s\n", opt);
                break;

                case 'a':
                    printf( "a %s\n", opt);
                break;

                case 'l':
                    printf( "l %s %s\n", opt, strtok(NULL, " "));
                break;


                default:
                    printf("Invalid option: %s\n", opt);
            }
        } else printf("Invalid option: %s\n", opt);
        
      opt = strtok(NULL, " ");
   }
}



void processOptionsLink(char options[]){
    char *opt;
    opt = strtok(options, " ");

    while(opt != NULL ) {
        if((strlen(opt) == 2 && opt[0] == '-')){
            switch(opt[1]){
                case 'n':
                    printf( "n %s\n", opt);
                break;

                case 'l':
                    printf( "l %s\n", opt);
                    return;
                break;

                case 'd':
                    printf( "d %s\n", opt);
                break;

                case 't':
                    printf( "t %s\n", opt);
                break;

                case 'a':
                    printf( "a %s\n", opt);
                break;

                default:
                    printf("Invalid option: %s\n", opt);
            }
        } else printf("Invalid option: %s\n", opt);
        
      opt = strtok(NULL, " ");
   }
}

void validateOptions(int type){
    char options[100];

    fgets(options, sizeof(options), stdin);

    int i = strlen(options) - 1;
    while(isspace(options[i])){
        options[i] = '\0';
        i--;
    }

    switch (type)
    {
    case 1:
        processOptionsFile(options);
        break;
    
    case 2:
        processOptionsLink(options);
        break;

    case 3:
        processOptionsDir(options);
        break;
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

        int type;

        if(S_ISREG(file.st_mode)){
            printf("-n show name\n-d show size\n-h show the hard link count\n-m show time of last modification\n-a show access rights\n-l create a symbolic link\n");
            type = 1;
        }

        if(S_ISLNK(file.st_mode)){
            printf("-n show name\n-l delete symbolic links\n-d show size of sybolic link\n-t show size of target file\n-a show access rights\n");
            type = 2;
        }
        
        if(S_ISDIR(file.st_mode)){
            printf("-n show name\n-d show size\n-a show access rights\n-c show total number of files with .c extension\n");
            type = 3;
            
        }
        validateOptions(type);

    }
}