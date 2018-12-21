// Contributor: Chelsea Young, Wayne Garman (makefile)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "cmdparse.h"

int main()
{
    // Mandatory for processing
    long lForkPid;
    // PID of child that terminated
    long lWaitPid;
    // Used by parent to get status of child
    int check;
    // Typedef Struct
    CMD cmd;
    // Input Buffer
    char szInputBuffer[100];

    printf("$ ");

    while(fgets(szInputBuffer, 100, stdin))
    {
        // Create a child process
        lForkPid = fork();

        check = cmdparse(szInputBuffer, &cmd);
        
        if(strcmp(szInputBuffer, "exit") == 0)
            exit(1);

        if(check == 1)
        {
            printf("Parsing failed.\n");
            exit(1);
        }

        // Both the parent and child continue here
        switch(lForkPid)
        {
            case -1:
            {
                printf("Fork failed.\n");
                exit(1);
            }
            // Child Process
            case 0:
            {
                check = execvp(cmd.argv1[0], cmd.argv1);

                if(check == -1)
                    printf("Child process failed to exec.\n");
                
                break;
            }
            // Parent Process
            default:
            {
                if(cmd.background == FALSE)
                    lWaitPid = waitpid(-1, 0, 0);
                
                printf("$ ");

                break;
            }
        }// End of Switch

        fflush(stdin);

    }// End of While

    return 0;
}
