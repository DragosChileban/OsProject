#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <dirent.h>
#include <regex.h>
#include <sys/wait.h>


struct stat inf; 
char choice[10];

void print_reg_menu() {
    printf("Menu for regular file \n");
    printf("1. Read -n \n");
    printf("2. Size -d \n");
    printf("3. Number of hard links -h \n");
    printf("4. Time of last modification -m \n");
    printf("5. Access rights -a \n");
    printf("6. Create a symbolic link give:link name -l \n");
}

void reg_options(char * choice, char * name) {
    for(int i = 1; i < strlen(choice); i++) {
        printf("%c ", choice[i]);
        if(choice[i] == 'n')
            printf("File name is %s \n", name);
        if(choice[i] == 'd')
            printf("File size is %ld\n", inf.st_size);
        if(choice[i] == 'h')
            printf("Number of hard links is %ld\n", inf.st_nlink);
        if(choice[i] == 'm')
            printf("Time of last modification %s\n",ctime(&inf.st_atime));
        if(choice[i] == 'a')
            printf("Access rights %d %d\n",inf.st_uid,inf.st_gid);
        if(choice[i] == 'l') {
            printf("Create a symbolic link give:link name -l \n");
            printf("Introduce the name of the new link\n");
            char link[100];
            scanf("%s",link);
            symlink(name,link);
        }
    }
}

void reg_read(char *name) {
    int is_valid;
    do {
        is_valid = 1;
        print_reg_menu();
        scanf("%s", choice);
        if(choice[0] != '-') {
            is_valid = 0;
            system("clear");
            printf("Your option choice is not valid!\n");
        }
        else {
            for(int i = 1; i < strlen(choice); i++)
                if(strchr("ndhmal", choice[i]) == NULL) {
                    is_valid = 0;
                    system("clear");
                    printf("Your option choice is not valid!\n");
                    break;
                }
        }
    } while(!is_valid);

    reg_options(choice, name);
}

void print_link_menu() {

    printf("Menu for symbolic link \n");
    printf("1. Link name -n \n");
    printf("2. Delete link -l \n");
    printf("3. Size of the link -d \n");
    printf("4. Size of the target -t \n");
    printf("5. Access rights -a \n");
}

void link_options(char * choice, char * name) {
    for(int i = 1; i < strlen(choice); i++) {
        printf("%c ", choice[i]);
        if(choice[i] == 'n')
            printf("Link name : %s \n",name);
        if(choice[i] == 'l') {
            unlink(name);   
            printf("Deleting the link : %s\n",name);  
        }
        if(choice[i] == 'd')
            printf("Size of the link %ld \n",inf.st_size);
        if(choice[i] == 't')  {
            struct stat sbNew;
            stat(name, &sbNew);
            printf("Size of the target %ld\n",sbNew.st_size);
        }
        if(choice[i] == 'a')
            printf("Access rights   %d %d\n",inf.st_uid,inf.st_gid);
    }
}

void link_read(char *name) {
    int is_valid;
    do {
        is_valid = 1;
        print_link_menu();
        scanf("%s", choice);
        if(choice[0] != '-') {
            is_valid = 0;
            system("clear");
            printf("Your option choice is not valid!\n");
        }
        else {
            for(int i = 1; i < strlen(choice); i++)
                if(!strchr("nldta", choice[i])) {
                    is_valid = 0;
                    system("clear");
                    printf("Your option choice is not valid!\n");
                    break;
                }
        }
    } while(!is_valid);

    link_options(choice, name);
}

void print_dir_menu() {
    printf("Menu for directory \n");
    printf("1. Directory name -n \n");
    printf("2. Size -d \n");
    printf("3. Access rights -a \n");
    printf("4. Total number of files with the .c extension -c \n");
}

void dir_options(char * choice, char * name) {
    for(int i = 1; i < strlen(choice); i++) {
        printf("%c ", choice[i]);
        if(choice[i] == 'n')
            printf("Directory name is %s \n", name);
        if(choice[i] == 'd')
            printf("Directo size is %ld\n", inf.st_size);
    }
}

void dir_read(char *name) {
    int is_valid;
    do {
        is_valid = 1;
        print_reg_menu();
        scanf("%s", choice);
        if(choice[0] != '-') {
            is_valid = 0;
            system("clear");
            printf("Your option choice is not valid!\n");
        }
        else {
            for(int i = 1; i < strlen(choice); i++)
                if(strchr("ndac", choice[i]) == NULL) {
                    is_valid = 0;
                    system("clear");
                    printf("Your option choice is not valid!\n");
                    break;
                }
        }
    } while(!is_valid);

    dir_options(choice, name);
}

void checkCfile(char *path) {
    regex_t extensionC;
    char buff[512];
    if(regcomp(&extensionC,".c$",REG_EXTENDED !=0)) {
        printf("Error compiling .c regular expression \n");
    }

    if(regexec(&extensionC,path, 0, NULL, 0) == 0) {
        int pfd[2];
        // int pid;
        if(pipe(pfd)<0) {
            perror("Pipe creation error\n");
            exit(1);
        }
        pid_t cpid = fork();
        if(cpid == -1) {
            perror("Fork failure \n");
            exit(EXIT_FAILURE);
        }
    
        if(cpid == 0) {
            close(pfd[0]); 
            dup2(pfd[1], 1);
            execlp("bash","bash","checkerr.sh",path,"errfile.txt",NULL);
            printf("!GOOOD");
            exit(1);     
        }  
        close(pfd[1]);
        read(pfd[0],buff,512);
        char *p;
        p = strtok(buff, " ");
        int nr_err = atoi(p);
        p = strtok(NULL, " ");
        int nr_war = atoi(p);
        printf("%d %d\n", nr_err, nr_war);
    }

}

void process_file(struct stat inf,char *path) {
   
    // pid_t pid = fork();
    // if(pid == -1) {
    //     perror("Fork failure \n");
    //     exit(EXIT_FAILURE);
    // }
    // if(pid == 0) {
    //     printf("Current file:  %s is a ", path);
 
    //     if (S_ISREG(inf.st_mode)) {
    //         printf("regular file\n");
    //         reg_read(path);
    //     }
    //     if(S_ISLNK(inf.st_mode)) {
    //         printf("symbolic link\n");
    //         link_read(path);
    //     }
    //     if(S_ISDIR(inf.st_mode)) {
    //         printf("directory\n");
    //     }
    // }

    checkCfile(path);

    wait(NULL);
    // wait(NULL);
}


int main(int argc, char * argv[]) {
 
    struct stat inf;
 
    for(int i = 1; i < argc; i++) {
        if (lstat(argv[i], &inf) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        process_file(inf, argv[i]);
    }
    return 0;
}