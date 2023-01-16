#include <stdio.h>
#include <unistd.h>



int main() {
	int pid = fork();
	char *arr[] = {"ls", "-lRa", NULL};
	if (pid == 0) {
		printf("I'm childish\n");
		execv("/bin/ls", arr);
	} else {
		printf("I'm the parent\n");
	}
}

