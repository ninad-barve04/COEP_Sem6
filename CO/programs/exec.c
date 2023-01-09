#include <stdio.h>
#include <unistd.h>

int main() {
	int pid = fork();
	if (pid == 0) {
		printf("I'm childish\n");
		execlp("ls", "ls", "-l", NULL);
	} else {
		printf("I'm the parent\n");
	}
}

