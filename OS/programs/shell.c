#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77


int main()
{
    char buffer[1024], cwd[64], prompt[64];
    char *temp;
    int pid;
    int custom = 0;
    int len;
    
    while (1) {
        if (custom == 0) {
            getcwd(cwd, 64);
            strcpy(prompt, cwd);
            printf("%s%s%s ", C_GREEN, cwd, C_RESET);
        } else {
            printf("%s%s%s ", C_GREEN, prompt, C_RESET);
        }
        
        scanf("%s", buffer);

        if (strstr(buffer, "PS1") != NULL) {
            temp = &(buffer[5]);
            strcpy(prompt, temp);
            len = strlen(prompt);
            prompt[len-1] = '\0';
            custom = 1;
            printf("PS1=%s\n",temp);
            // free(temp);
            continue;
        }

        // switch (buffer[0]) {
        //     case KEY_UP:
        //         printf("UP\n");
        //         break;
        //     case KEY_DOWN:
        //         printf("DOWN\n");
        //         break;
        //     case KEY_LEFT:
        //         printf("LEFT\n");
        //         break;
        //     case KEY_RIGHT:
        //         printf("RIGHT\n");
        //         break;
        //     default:
        //         break;
        // }
        
        if (strcmp(buffer, "exit") == 0) {
            printf("Exitting Gracefully!!\n");
            exit(0);
            break;
        }
        
        if (strcmp(buffer, "cd") == 0) {
            char path[64];
            scanf("%s", path);
            int c = chdir(path);
            continue;
        }

        pid = fork();
        if (pid == 0) {
            execlp(buffer, buffer, NULL);
        } else {
            if (strcmp(buffer, "exit") == 0) {
                exit(0);
                break;
            }
            wait(0);
        }
    }
    return 0;
}

