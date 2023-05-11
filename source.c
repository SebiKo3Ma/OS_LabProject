#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

 struct stat file;
 char filename[50];

void printName(){
    printf("Name: %s\n", filename);
}

void printSize(){
    printf("Size: %ld Bytes\n", file.st_size);
}

void hardLinkNo(){
    printf("Number of Hard Links: %ld\n", file.st_nlink);
}

void lastModified(){
    char mtime[80];
    time_t t = file.st_mtime;
    struct tm lt;
    localtime_r(&t, &lt);
    strftime(mtime, sizeof mtime, "%a, %d %b %Y %T", &lt);
    printf("Time of last modification: %s\n", mtime);
}

void accessRights(){
    printf("User:\nRead - %s\nWrite - %s\nExec - %s\n\nGroup:\nRead - %s\nWrite - %s\nExec - %s\n\nOthers:\nRead - %s\nWrite - %s\nExec - %s\n\n", 
    (file.st_mode & S_IRUSR) ? "yes" : "no",
    (file.st_mode & S_IWUSR) ? "yes" : "no",
    (file.st_mode & S_IXUSR) ? "yes" : "no",
    (file.st_mode & S_IRGRP) ? "yes" : "no",
    (file.st_mode & S_IWGRP) ? "yes" : "no",
    (file.st_mode & S_IXGRP) ? "yes" : "no",
    (file.st_mode & S_IROTH) ? "yes" : "no",
    (file.st_mode & S_IWOTH) ? "yes" : "no",
    (file.st_mode & S_IXOTH) ? "yes" : "no"
    );
}

void createSymLink(char *name){
    int symLink = symlink(filename, name);
    if(symLink == 0)
        printf("Symbolic link created succesfully!\n");
        else printf("Error creating Symbolic link\n");
}

void processOptionsFile(char options[]){
    char *opt;
    char *name;
    opt = strtok(options, " ");

    while(opt != NULL ) {
        if((strlen(opt) == 2 && opt[0] == '-')){
            switch(opt[1]){
                case 'n':
                    printName();
                break;

                case 'd':
                    printSize();
                break;

                case 'h':
                    hardLinkNo();
                break;

                case 'm':
                    lastModified();
                break;

                case 'a':
                    accessRights();
                break;

                case 'l':
                    name = strtok(NULL, " ");
                    createSymLink(name);
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
                    printName();
                break;

                case 'l':
                    printf( "l %s\n", opt);
                    return;
                break;

                case 'd':
                    printSize();
                break;

                case 't':
                    printf( "t %s\n", opt);
                break;

                case 'a':
                    accessRights();
                break;

                default:
                    printf("Invalid option: %s\n", opt);
            }
        } else printf("Invalid option: %s\n", opt);
        
      opt = strtok(NULL, " ");
   }
}


void processOptionsDir(char options[]){
    char *opt;
    opt = strtok(options, " ");

    while(opt != NULL ) {
        if((strlen(opt) == 2 && opt[0] == '-')){
            switch(opt[1]){
                case 'n':
                    printName();
                break;

                case 'd':
                    printSize();
                break;

                case 'a':
                    accessRights();
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
        if(lstat(argv[i], &file)){
            perror("Error!\n");
            exit(2);
        }

        int type;

        if(S_ISREG(file.st_mode)){
            printf("\nREGULAR FILE:\n-n show name\n-d show size\n-h show the hard link count\n-m show time of last modification\n-a show access rights\n-l create a symbolic link\n");
            type = 1;
        }

        if(S_ISLNK(file.st_mode)){
            printf("\nSYMBOLIC LINK:\n-n show name\n-l delete symbolic links\n-d show size of sybolic link\n-t show size of target file\n-a show access rights\n");
            type = 2;
        }
        
        if(S_ISDIR(file.st_mode)){
            printf("\nDIRECTORY:\n-n show name\n-d show size\n-a show access rights\n-c show total number of files with .c extension\n");
            type = 3;
            
        }
        strcpy(filename, argv[i]);
        validateOptions(type);

    }
}