#include <stdio.h>
#include <unistd.h>
int main() {
	int pid = fork();
	if (pid == 0) {
		printf("child %d\n", getpid());
	} else {
		printf("parent %d\n", getpid());
	}
}
