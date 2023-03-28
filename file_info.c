#include <sys/types.h>
#include<time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
 
 
int main(int argc, char * argv[]) {
 
    struct stat inf;
    char choice[10];
 
    // printf("%d arguments\n", argc);
 
    for(int i = 1; i < argc; i++) {
        printf("Current file:  %s is a ", argv[i]);
        if(lstat(argv[i],&inf) == -1) {
            perror("lstat() error");
            exit(EXIT_FAILURE);
        }
 
        if (S_ISREG(inf.st_mode)) {
            printf("regular file\n");
            printf("Menu for regular file \n");
            printf("1. Read -n \n");
            printf("2. Size -d \n");
            printf("3. Number of hard links -h \n");
            printf("4. Time of last modification -m \n");
            printf("5. Access rights -a \n");
            printf("6. Create a symbolic link give:link name -l \n");
            scanf("%s", choice);
            for(int i = 1; i < strlen(choice); i++) {
                printf("%c ", choice[i]);
                if(choice[i] == 'n')
                    printf("File name is %s \n", argv[i]);
                if(choice[i] == 'd')
                    printf("File size is %ld\n", inf.st_size);
                if(choice[i] == 'h')
                    printf("Number of hard links is %ld", inf.st_nlink);
            }
            printf("\n");
            // system("clear");
 
        }
        if(S_ISLNK(inf.st_mode)) {
            printf("symbolic link\n");
            printf("Menu for symbolic link \n");
            printf("1. Link name -n \n");
            printf("2. Delete link -l \n");
            printf("3. Size of the link -d \n");
            printf("4. Size of the target -t \n");
            printf("5. Access rights -a \n");
            scanf("%s", choice);
            for(int i = 1; i < strlen(choice); i++)
                printf("%c ", choice[i]);
            printf("\n");
            // system("clear");
        }
    }
    return 0;
}