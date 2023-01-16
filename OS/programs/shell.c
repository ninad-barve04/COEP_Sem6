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
    char *args[1024];
    char *temp;
    int pid;
    int custom = 0;
    int len;
    int status = 1;
    
    char PATH[]="/usr/bin/";

    while (status) {
        /*for displaying the prompt if it is the cwd or a custom one*/
        if (custom == 0) {
            getcwd(cwd, 64);
            strcpy(prompt, cwd);
            printf("%s%s%s$ ", C_GREEN, cwd, C_RESET);
        } else {
            printf("%s%s%s$ ", C_GREEN, prompt, C_RESET);
        }
        /*Getting the user command shell*/
        fgets(buffer, 1024, stdin);
        buffer[strlen(buffer)-1] = '\0';

        /*setting the custom prompt*/
        if (strstr(buffer, "PS1") != NULL) {
            temp = &(buffer[5]);
            strcpy(prompt, temp);
            len = strlen(prompt);
            prompt[len-1] = '\0';
            if (strcmp(prompt, "\\w$") == 0) {
                custom = 0;
            } else {
                custom = 1;
            }
            printf("PS1=%s\n",temp);
            continue;
        }
        /*Gracefully exiting the shell*/
        else if (strcmp(buffer, "exit") == 0) {
            printf("Exitting Gracefully!!\n");
            status = 0;
            break;
        }
        /*handling changing directory*/
        else if (strstr(buffer, "cd") != NULL) {
            temp = &(buffer[3]);
            char path[64];
            strcpy(path, temp);
            int c = chdir(path);
            continue;
        } else {
            /*adding PATH to the command*/
            temp = (char *)malloc(strlen(PATH));
            strcpy(temp, PATH);
            strcat(temp, buffer);
            strcpy(buffer, temp);
            free(temp);
            // printf("Command = %s\n", buffer);

            /*Now creating an argument array for execv syscall*/
            char *p = strtok(buffer, " ");
            int i = 0;
            while (p != NULL) {
                args[i++] = p;
                p = strtok(NULL, " ");
            }
            args[i] = NULL;

            pid = fork();
            if (pid == 0) {
                execv(buffer, args);
            } else {
                if (strcmp(buffer, "exit") == 0) {
                    // exit(0);
                    // break;
                    status = 0;
                } 
                wait(0);
                
            }
        }
    }
    return 0;
}

