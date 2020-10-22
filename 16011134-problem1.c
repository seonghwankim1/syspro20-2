#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

	printf("Child PID is %d, Parent PID is %d.\n", getppid(), getpid());

	printf("Parent PID is %d\n", getpid());

	return 0;
}
