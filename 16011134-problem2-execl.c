#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	pid_t pid;

	pid = fork();//process created

	if(pid == 0)//child process
	{
		execl("/bin/ls", "ls", "-al", (char *)0);
		//execl(path, arg0, arg1, NULL)
		perror("execl() failed\n"); //if execl() fail
		printf("%d", pid);
	}
	else if(pid > 0) //parent process
	{
		wait((int *)0);
		printf("execl()function execution\n");
		//execl() is completed in child
		exit(0);
	}
	return 0;
}
