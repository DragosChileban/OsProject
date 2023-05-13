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


char choice[10];
int nr_childs;

void access_rights(struct stat inf) {
    printf("User: \n \n");
    printf("-Read - ");
    if (inf.st_mode & S_IRUSR)
        printf("yes\n");
    else
        printf("no\n");
    printf("-Write - ");
    if (inf.st_mode & S_IWUSR)
        printf("yes\n");
    else
        printf("no\n");
    printf("-Execute - ");
    if (inf.st_mode & S_IXUSR)
        printf("yes\n");
    else
        printf("no\n");

    
    printf("\nGroup: \n \n");
    printf("-Read - ");
    if (inf.st_mode & S_IRGRP)
        printf("yes\n");
    else
        printf("no\n");
    printf("-Write - ");
    if (inf.st_mode & S_IWGRP)
        printf("yes\n");
    else
        printf("no\n");
    printf("-Execute - ");
    if (inf.st_mode & S_IXGRP)
        printf("yes\n");
    else
        printf("no\n");


    printf("\nOthers: \n \n");
    printf("-Read - ");
    if (inf.st_mode & S_IROTH)
        printf("yes\n");
    else
        printf("no\n");
    printf("-Write - ");
    if (inf.st_mode & S_IWOTH)
        printf("yes\n");
    else
        printf("no\n");
    printf("-Execute - ");
    if (inf.st_mode & S_IXOTH)
        printf("yes\n");
    else
        printf("no\n");
}

void print_reg_menu() {
    printf("Menu for regular file \n");
    printf("1. Read -n \n");
    printf("2. Size -d \n");
    printf("3. Number of hard links -h \n");
    printf("4. Time of last modification -m \n");
    printf("5. Access rights -a \n");
    printf("6. Create a symbolic link give:link name -l \n");
}

void reg_options(struct stat inf, char * choice, char * name) {
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
        if(choice[i] == 'a') {
            printf("Access rights %d %d\n",inf.st_uid,inf.st_gid);
            access_rights(inf);
        }
        if(choice[i] == 'l') {
            printf("Create a symbolic link give:link name -l \n");
            printf("Introduce the name of the new link\n");
            char link[100];
            scanf("%s",link);
            if (symlink(name, link) == -1) {
                fprintf(stderr, "ERROR!\n");
                fprintf(stderr, "Could not create the symbolic link!");
                exit(1);
            }
        }
    }
    printf("\n");
}

void reg_read(struct stat inf, char *name) {
    int is_valid;
    do {
        is_valid = 1;
        print_reg_menu();
        scanf("%s", choice);
        if(choice[0] != '-') {
            is_valid = 0;
            // system("clear");
            printf("Your option choice is not valid!\n");
        }
        else {
            for(int i = 1; i < strlen(choice); i++)
                if(strchr("ndhmal", choice[i]) == NULL) {
                    is_valid = 0;
                    // system("clear");
                    printf("Your option choice is not valid!\n");
                    break;
                }
        }
    } while(!is_valid);

    reg_options(inf, choice, name);
}

void print_link_menu() {

    printf("Menu for symbolic link \n");
    printf("1. Link name -n \n");
    printf("2. Delete link -l \n");
    printf("3. Size of the link -d \n");
    printf("4. Size of the target -t \n");
    printf("5. Access rights -a \n");
}

void link_options(struct stat inf, char * choice, char * name) {
    
    for(int i = 1; i < strlen(choice); i++) {
        printf("%c ", choice[i]);
        if(choice[i] == 'n')
            printf("Link name : %s \n",name);
        if(strchr(choice, 'l')) {
            unlink(name);   
            printf("Deleting the link : %s\n",name);
            break;
        }
        if(choice[i] == 'd')
            printf("Size of the link %ld \n",inf.st_size);
        if(choice[i] == 't')  {
            struct stat sbNew;
            stat(name, &sbNew);
            printf("Size of the target %ld\n",sbNew.st_size);
        }
        if(choice[i] == 'a') {
            printf("Access rights   %d %d\n",inf.st_uid,inf.st_gid);
            access_rights(inf);
        }
    }
    
}

void link_read(struct stat inf, char *name) {
    int is_valid;
    do {
        is_valid = 1;
        print_link_menu();
        scanf("%s", choice);
        if(choice[0] != '-') {
            is_valid = 0;
            // system("clear");
            printf("Your option choice is not valid!\n");
        }
        else {
            for(int i = 1; i < strlen(choice); i++)
                if(!strchr("nldta", choice[i])) {
                    is_valid = 0;
                    // system("clear");
                    printf("Your option choice is not valid!\n");
                    break;
                }
        }
    } while(!is_valid);

    link_options(inf, choice, name);
}

int nr_c_files(DIR *dir, char *path) {
    int nr_c = 0;
    struct dirent *entry;
    struct stat file;
    regex_t extension;

    if(regcomp(&extension, ".c$", REG_EXTENDED)) {
        printf("Error compiling regular expression\n");
        exit(1);
    }
 
    while ((entry = readdir(dir))!=NULL) {
        char *name=entry->d_name;
        char  auxpath[4096];
        if(strlen(name) > 2) {
            
            strcpy(auxpath,path);
            strcat(auxpath,name);
            
            if(lstat(auxpath,&file) == -1) {
                printf("Error could not get the file stats");
                exit(EXIT_FAILURE);
            }
            if (S_ISREG(file.st_mode) && regexec(&extension, entry->d_name, 0, NULL, 0) == 0) {
                nr_c++;
            }
    }
  }
   closedir(dir);   
   return nr_c;  
  
}

void print_dir_menu() {
    printf("Menu for directory \n");
    printf("1. Directory name -n \n");
    printf("2. Size -d \n");
    printf("3. Access rights -a \n");
    printf("4. Total number of files with the .c extension -c \n");
}

void dir_options(struct stat inf, char * choice, char * name) {
    DIR *dir;
    
    for(int i = 1; i < strlen(choice); i++) {
        printf("%c ", choice[i]);
        if(choice[i] == 'n')
            printf("Directory name is %s \n", name);
        if(choice[i] == 'd')
            printf("Directo size is %ld\n", inf.st_size);
        if(choice[i] == 'a') {
            printf("Access rights   %d %d\n",inf.st_uid,inf.st_gid);
            access_rights(inf);
        }
        if(choice[i] == 'c') {
            dir = opendir(name);
            if(dir == NULL) {
                printf("Error! Could not open the directory");
            }
            strcat(name,"/");
            int nr_c = nr_c_files(dir, name);
            printf("Total number of files with .c extension : %d \n",nr_c);
        }
    }
}

void dir_read(struct stat inf, char *name) {
    int is_valid;
    do {
        is_valid = 1;
        print_dir_menu();
        scanf("%s", choice);
        if(choice[0] != '-') {
            is_valid = 0;
            // system("clear");
            printf("Your option choice is not valid!\n");
        }
        else {
            for(int i = 1; i < strlen(choice); i++)
                if(strchr("ndac", choice[i]) == NULL) {
                    is_valid = 0;
                    // system("clear");
                    printf("Your option choice is not valid!\n");
                    break;
                }
        }
    } while(!is_valid);

    dir_options(inf, choice, name);
}

int compute_score(int err, int war) {
    int score;
    if(!err && !war)
        score = 10;
    if(err)
        score = 1;
    else if(war > 10)
            score = 2;
    else
        score = 2 + 8*(10 - war)/10;
    return score;
}

int check_file(struct stat inf, char *path) {
    regex_t extensionC;
    char buff[512];
    if(regcomp(&extensionC,".c$",REG_EXTENDED !=0)) {
        printf("Error compiling .c regular expression \n");
    }

    if(regexec(&extensionC,path, 0, NULL, 0) == 0) {
        int pfd[2];
        if(pipe(pfd)<0) {
            perror("Pipe creation error\n");
            exit(69);
        }
        pid_t cpid = fork();
        if(cpid == -1) {
            perror("Fork failure \n");
            exit(EXIT_FAILURE);
        }
        nr_childs++;
    
        if(cpid == 0) {
            close(pfd[0]); 
            dup2(pfd[1], 1);
            execlp("sh","sh","checkerr.sh",path,NULL);
            printf("not good");
            exit(1);     
        }  
        char tmp[1];
        close(pfd[1]);
        // read(pfd[0],buff,512);
        while (read(pfd[0], tmp, 1) == 1) {
            strncat(buff, tmp, 1);
        }
        close(pfd[0]);
        // printf("buff   %s", buff);
        
        char *p;
        p = strtok(buff, " ");
        int nr_err = atoi(p);
        p = strtok(NULL, " ");
        int nr_war = atoi(p);
        
        return compute_score(nr_err, nr_war);
        // printf("errors %d warnings %d\n", nr_err, nr_war);
    }
    else {
        if(S_ISREG(inf.st_mode)) {
            pid_t cpid = fork();
            if(cpid == -1) {
                perror("Fork failure \n");
                exit(EXIT_FAILURE);
            }
            nr_childs++;
            if(cpid == 0) {
                execlp("wc", "wc", "-l", path, NULL);
                printf("not good");
                exit(1);     
            }  
        }
        if(S_ISDIR(inf.st_mode)) {
            // printf("the path is %s\n", path);
            char cpath[100];
            if(path[strlen(path)-1] == '/')
                path[strlen(path)-1] = '\0';
            strcpy(cpath, path);
            strcat(cpath, "/");
            strcat(cpath, path);
            strcat(cpath, "_file.txt");
            pid_t cpid = fork();
            if(cpid == -1) {
                perror("Fork failure \n");
                exit(EXIT_FAILURE);
            }
            nr_childs++;
            if(cpid == 0) {
                
                execlp("/bin/touch", "touch", cpath, NULL);
                printf("not good");
                exit(1);     
            }  
        }
        if(S_ISLNK(inf.st_mode)) {
            pid_t cpid = fork();
            if (cpid == -1) {
                perror("Fork failure \n");
                exit(EXIT_FAILURE);
            }
            nr_childs++;
            if (cpid == 0) {
                execlp("chmod", "chmod", "-v", "760", path, NULL);
                printf("not good");
                exit(1);
            }
        }
    }

    return 0;

}

void check_menu(struct stat inf,char *path) {
    pid_t pid = fork();
    if(pid == -1) {
        perror("Fork failure \n");
        exit(EXIT_FAILURE);
    }
    nr_childs++;
    if(pid == 0) {
        printf("Current file:  %s is a ", path);
 
        if (S_ISREG(inf.st_mode)) {
            printf("regular file\n");
            reg_read(inf, path);
        }
        if(S_ISLNK(inf.st_mode)) {
            printf("symbolic link\n");
            link_read(inf, path);
        }
        if(S_ISDIR(inf.st_mode)) {
            printf("directory\n");
            dir_read(inf, path);
        }
        exit(0);
    }
}

void process_file(struct stat inf,char *path) {
   
    check_menu(inf, path);
    int score = check_file(inf, path);
    
    // printf("childs   %d\n", nr_childs);
    for(int i = 0; i < nr_childs; i++) {
        int status;
        pid_t w;
        w = wait(&status);
        if(w == -1)
            exit(70);
        if (WIFEXITED(status)) {
            printf("The process with PID %d has ended with the exit code %d\n", w, WEXITSTATUS(status));
        }
    }
    if(score>0) {
        FILE *f;
        f = fopen("grades.txt", "a");
        fprintf(f, "<%s>: <%d>\n", path, score);
        fclose(f);
        // printf("The score is: %d\n", score);
        // printf("\n\n");
    }
    
}


int main(int argc, char * argv[]) {
 
    struct stat inf;
 
    for(int i = 1; i < argc; i++) {
        if (lstat(argv[i], &inf) == -1) {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        nr_childs = 0;
        process_file(inf, argv[i]);
    }
    return 0;
}