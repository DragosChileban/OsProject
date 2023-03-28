#include <sys/types.h>
#include<time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[]) {

    struct stat inf;

    printf("%d arguments\n", argc);

    for(int i = 1; i < argc; i++) {
        if(lstat(argv[i],&inf) == -1) {
            exit(EXIT_FAILURE);
            perror("lstat() error");
        }
        printf("%s\n", argv[i]);
    }
    return 0;
}

//C:\Users\Andre\Desktop\UPT\Anul 2 sem 2\OS\OsProject\readme.md
//C:\Users\Andre\Desktop\UPT\Anul 2 sem 2\SEF\test 1\Capture.JPG