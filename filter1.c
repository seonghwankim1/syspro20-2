#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEF_PAGER "/vin/more"
#define MAXLINE 1024

int main(int argc, char *argv[]) {
	int n, fd[2];
	char *pager, *argv0;
	char line[MAXLINE];
	FILE *fp;

	pid_t pid;

	if(argc != 2) perror("Usage : filter1 FILE");
	if((fp = fopen(argv[1], "r")) == NULL) perror("fopen error");
	if(pipe(fd) < 0) perror("pipe error");

	if((pid = fork()) < 0) perror("fork error");

	/* parent part */
	else if(pid >0) {
		close(fd[0]); //close the read end of the pipe

		/* parent copis from argv[1] to the pipe */
		while(fgets(line, MAXLINE, fp) != NULL) {
			n = strlen(line);

			if(write(fd[1], line, n) != n)
				perror("write error");
		}

		if(ferror(fp))
			perror("fgets error");

		close(fd[1]); //close write end of pipe for reader

		if(waitpid(pid, NULL, 0) < 0)
			perror("waitpid error");
	}
	else {
		
		close(fd[1]); //close the write end

		if(fd[0] != STDIN_FILENO) {

			//dup2() does nothing if the new and old FDs ard the same
			if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
					perror("dup2 error");

			close(fd[0]); //no longer needed
		}

		/* get arguments for execl() */
		if((pager = getenv("PAGER")) == NULL)
				pager = DEF_PAGER;

		if((argv0 = strrchr(pager, '/')) != NULL)
				argv0++; //step past rightmost slash
		else
				argv0 = pager; //no slash in pager

		//here, we now call the pager(/bin/more)
		//to print the file (argv0) in the screen
		if(execl(pager, argv0, (char *)0) <0)
				perror("execl error");
	}
	exit(EXIT_SUCCESS);
}


