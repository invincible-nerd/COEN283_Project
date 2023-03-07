#include <stdio.h>

int main(){
    printf("\n\n********************** Welcome to file system - group 10086 !!! **********************\n\n");
    printf("Instruction on how to use this file system: \n");
    printf("\tTo initialize the fs - 10086      \t\tinit                                     \n");
    printf("\tTo show the current path          \t\tshowpath                                 \n");
    printf("\tTo jump into a directory          \t\tjumpinto   [DIR-PATH] or [DIR-NAME]      \n");
    printf("\tTo create a file                  \t\tcreate     [FILE-NAME] [FILE-SIZE]       \n");
    printf("\tTo create a directory             \t\tcreatedir  [DIR-NAME]                    \n");
    printf("\tTo remove a file                  \t\tremove     [FILE-NAME]                   \n");
    printf("\tTo remove a directory             \t\tremovedir  [DIR-NAME]                    \n");
    printf("\tTo write something to a file      \t\twrite      [FILE-NAME] [CONTENT]         \n");
    printf("\tTo show content of a file/dir     \t\tshow       [FILE-NAME]                   \n");
    printf("\tTo list all saved files           \t\tlistfiles                                \n");
    printf("\tTo show basic structures of the fs\t\tprintfs                                  \n");
    printf("\tTo clear the disk                 \t\tclearfs                                  \n");
    printf("\n\n");
    printf("Sample to start: \n");
    printf("    1. initialize the file system:          init                             \n");
    printf("    2. create a file:                       create  file1   3000             \n");
    printf("    3. write something to a file:           write   file1   \"something\"    \n");
    printf("    4. check the content of a file:         show    file1                    \n");
    printf("    .....\n");
    printf("    *everytime after you clear the disk, please make sure reinitialize it before using\n");
    printf("\n\n");
}