/*Write a C program that receives as parameters multiple arguments representing
paths to regular files, directories or symbolic links. Depending on the file type, print an interactive menu:
A) regular file
-n (file name)
-d (dimens)
-h (no of hard links)
-m (time of modif)
-a (access rights)
-l (create a symlink, give a link name)*/

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
}