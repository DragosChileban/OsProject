#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{   

    regex_t extension,extensionC;
    char *validCommands;

    if (S_ISREG(sb.st_mode))
    {
        //Call the regular file menu
        regFileMenu(sb,path);
        
        if(regcomp(&extensionC,".c$",REG_EXTENDED !=0))
        {
        printf("Error compiling .c regular expression \n");
        }

        if(regexec(&extensionC,path, 0, NULL, 0) == 0)
        {
            pid_t cpid = fork();
            int wstatus;
            if(cpid == -1)
            {
                perror("Fork failure \n");
                exit(EXIT_FAILURE);


            }
            if(cpid == 0)
            {
                //Child code
                    
            }
            

        }


            //Assign the valid commands for the regular file
            validCommands = "-ndhmal";

            //Compile the regular expression for the regular file
            if(regcomp(&extension,"^-[ndhmal]+$",REG_EXTENDED) != 0)
            {
                
                printf("Error compiling the regular expression \n");
            }
            
        }

    pid_t cpid, w;
    int wstatus;

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {            /* Code executed by child */
        printf("Child PID is %ld\n", (long) getpid());
        if (argc == 1)
            pause();                    /* Wait for signals */
        _exit(atoi(argv[1]));

    } else {                    /* Code executed by parent */
        do {
                        w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
                        if (w == -1) {
                            perror("waitpid");
                            exit(EXIT_FAILURE);
                        }

                        if (WIFEXITED(wstatus)) {
                            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
                        } else if (WIFSIGNALED(wstatus)) {
                            printf("killed by signal %d\n", WTERMSIG(wstatus));
                        } else if (WIFSTOPPED(wstatus)) {
                            printf("stopped by signal %d\n", WSTOPSIG(wstatus));
                        } else if (WIFCONTINUED(wstatus)) {
                            printf("continued\n");
                        }
                    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
                    exit(EXIT_SUCCESS);
                }
            }
