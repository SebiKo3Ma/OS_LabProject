#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

 struct stat file;
 char filepath[50];

void printName(){
    char *name = strrchr(filepath,'/');
    printf("Name: %s\n", name == NULL ? filepath : name + 1);
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
    int symLink = symlink(filepath, name);
    if(symLink == 0)
        printf("Symbolic link created succesfully!\n");
        else printf("Error creating Symbolic link\n");
}

void deleteSymLink(){
    unlink(filepath);
    printf("Symbolic link deleted!\n");
}

void targetFileSize(){
    struct stat target;
    if(stat(filepath, &target)){
            perror("Error!\n");
            exit(2);
        }
    printf("Size: %ld Bytes\n", target.st_size);
}

bool checkCFile(){
    char *ext = strrchr(filepath,'.');
    if(ext && !strcmp(ext,".c")){
        return true;
        }
    return false;
}

void countCFiles(){
    int counter = 0;
    DIR * dir;
    if((dir = opendir(filepath)) <0 ){
        perror("Cannot open folder!\n");
        exit(1);
    }
    
    struct dirent *entry;
    struct stat infoEnt;
    char path[500];
    while((entry = readdir(dir)) != NULL){
        sprintf(path, "%s/%s", filepath, entry->d_name);
        if(lstat(path, &infoEnt)){
            printf("Invalid file!");
        }

        if(S_ISREG(infoEnt.st_mode)){
            char *ext = strrchr(entry->d_name,'.');
            if(ext && !strcmp(ext,".c")){
                counter++;
            }
        }
        
    }
    printf("Number of C files: %d\n", counter);
    closedir(dir);
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
                    deleteSymLink();
                    return;
                break;

                case 'd':
                    printSize();
                break;

                case 't':
                    targetFileSize();
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
                    countCFiles();
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

    sleep(2);
    printf("Input options:");
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

void firstChildProcess(){
    int type;

            printName();
            if(S_ISREG(file.st_mode)){
                printf("REGULAR FILE:\n-n show name\n-d show size\n-h show the hard link count\n-m show time of last modification\n-a show access rights\n-l create a symbolic link\n\n");
                type = 1;
            }

            if(S_ISLNK(file.st_mode)){
                printf("SYMBOLIC LINK:\n-n show name\n-l delete symbolic links\n-d show size of sybolic link\n-t show size of target file\n-a show access rights\n\n");
                type = 2;
            }
            
            if(S_ISDIR(file.st_mode)){
                printf("DIRECTORY:\n-n show name\n-d show size\n-a show access rights\n-c show total number of files with .c extension\n\n");
                type = 3;
                
            }
            validateOptions(type);
}


void printLineNumber(){
    char command[100];
    sprintf(command, "wc -l < %s", filepath);

    FILE *pipe;
    char output[100];

    pipe = popen(command, "r");
    if (pipe == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    while (fgets(output, sizeof(output), pipe) != NULL) {
        printf("Line number: %s\n", output);
    }

    pclose(pipe);
}

void secondChildProcess(){
    if(S_ISREG(file.st_mode)){
                if(checkCFile()) printf("Script!\n");
                else printLineNumber();
            }

            if(S_ISLNK(file.st_mode)){
                printf("Change permissions\n");
            }
            
            if(S_ISDIR(file.st_mode)){
                printf("Create text file\n");
            }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        perror("Invalid number of arguments!");
        exit(0);
    }

    int PID, status;

    for(int i = 1; i < argc; i++){
       
        if(lstat(argv[i], &file)){
            perror("Error!\n");
            exit(2);
        }

        strcpy(filepath, argv[i]);
        printf("\n");

        if((PID = fork()) < 0){
            perror("Failed to create process!\n");
                exit(i);
        }
        else if(PID == 0){
            firstChildProcess();
            exit(i);
            sleep(1);
        }

        if((PID = fork()) < 0){
            perror("Failed to create process!\n");
            exit(i);
        }
        else if(PID == 0){
            secondChildProcess();
            exit(i);
        }

        sleep(1);
        int PID_Child;
        PID_Child = wait(&status);
        if(PID_Child < 0){
            perror("Error!");
            exit(i);
        }
        if(WIFEXITED(status)){
            printf("Process with PID %d ended with status %d\n\n", PID_Child, WIFEXITED(status));
        }

        PID_Child = wait(&status);
        if(PID_Child < 0){
            perror("Error!");
            exit(i);
        }
        if(WIFEXITED(status)){
            printf("Process with PID %d ended with status %d\n", PID_Child, WIFEXITED(status));
        }

        sleep(2);
    }
}
