#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "cmdparse.h" 
#include <sys/wait.h>

int main()
{
	long lForkPid;
	long lwaitPid;
	char szBuffer[151];
	char szInput[51];
	int iParseCmd;
	CMD cmd;

	//While loop 
	while (1)
	{	
		printf("$ ");
		// get the input from the user 
		fgets (szBuffer, sizeof(szBuffer),stdin);
		sscanf (szBuffer, "%s", szInput);

		//if it the command is exit : exit the shell
		if (strcmp (szInput, "exit") == 0)
			return 0;

		//parse cmd using the provided function
		iParseCmd = cmdparse(szBuffer,&cmd);

		if (iParseCmd != 1)
		{
			// create a child process
			lForkPid = fork();

			switch(lForkPid)
			{
				case -1:
					//errExit("fork failed: %s", strerror(errno));
					fprintf(stderr,"Fork failed.\n");
					break;
				//if it is the child process, run the code in this case 0
				case 0:  // child process
					//Execute a cmd
					execvp (cmd.argv1[0],cmd.argv1);

					//errExit("Child process failed to exec: %s", strerror(errno));
					fprintf(stderr,"%s: command not found.\n",cmd.argv1[0]);

					break;
				default: // parent process

					if (cmd.background == 1){
						break;
					}
					lwaitPid = waitpid (-1,0,0);
					break;
					//errExit("wait error: %s", strerror(errno));
			}
			fflush(stdin);
		}
		else
		{
			fprintf(stderr, "Parse Error.\n");
		}

	}

	return 0;
}	

