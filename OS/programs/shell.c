#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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

#define NOR 0 /*No Redirection */
#define IPR 1 /*Input Redirection*/
#define OPR 2 /*Output Redirection*/
#define ORA 4 /*Output Redirection with Append*/



/*utility function definitions*/
int check_path(char **patharr, int path_count, char *cmd);
void remove_leading_and_trailing_space(char *dest, char *src);




int main()
{
    char buffer[1024], cwd[64], prompt[64], path[64], redfile[256], cmd[1024];
    char *args[1024];
    char *temp;
    int pid;
    int custom = 0;
    int len;
    int status = 1;
    int red_flag = NOR;
    
    char *PATHS[64], PATH[1024];
    PATHS[0]="/usr/bin/";
    int path_count = 1;
    

    while (status) {
        /*for displaying the prompt if it is the cwd or a custom one*/
        if (custom == 0) {
            getcwd(cwd, 64);
            strcpy(prompt, cwd);
        }    
        printf("%s%s%s$ ", C_GREEN, prompt, C_RESET);
        
        /*Getting the user command shell*/
        if (fgets(buffer, 1024, stdin) == NULL) {
            /*checking for ctrl-D*/
            printf("Exitting Gracefully!!\n");
            status = 0;
            break;
        }
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
            strcpy(path, temp);
            if (strlen(path) == 0) {
                strcpy(path, "/home/ninad");
            }
            int c = chdir(path);
            continue;
        } 
        /*handling changing the path*/
        else if (strstr(buffer, "PATH=") != NULL) {
            temp = &(buffer[5]);
            strcpy(PATH, temp);
            char *p = strtok(PATH, ":");
            while (p != NULL) {
                PATHS[path_count++] = p;
                p = strtok(NULL, ":");
            }
            printf("PATH set to ");
            for (int k = 0; k < path_count; k++) {
                printf("%s\t", PATHS[k]);
            }
            printf("\n");
            continue;
        } else {
            /*checking existence pf any paths*/
            if (path_count == 0) {
                printf("Please set PATH variable\n");
                printf("Syntax: PATH=path1:path2:...\n");
                continue;
            }
            /*adding PATH to the command*/
            else {
                /*removing leading whitespaces*/
                temp = (char *)malloc(strlen(buffer));
                strcpy(temp, buffer);
                remove_leading_and_trailing_space(buffer, temp);

                /*Checking for redirection*/
                if (strstr(buffer, "<") != NULL) {
                    red_flag = IPR;
                } else if (strstr(buffer, ">") != NULL) {
                    if (strstr(buffer, ">>") != NULL) {
                        red_flag = ORA;
                    } else {
                        red_flag = OPR;
                    }
                } else {
                    red_flag = NOR;
                }


                /*Now creating an argument array for handling options*/
                char *p = strtok(buffer, " ");
                int i = 0, idx;
                while (p != NULL) {
                    args[i++] = p;
                    p = strtok(NULL, " ");
                    if (p != NULL && (strcmp(p, ">") == 0 || strcmp(p, ">>") == 0 || strcmp(p, "<") == 0)) {
                        p = strtok(NULL, " ");
                        // printf("%s\n", p);
                        strcpy(redfile, p);
                        break;
                    }
                }
                args[i] = NULL;
                
                // int j = 0;
                // while (j <= i) {
                //     printf("%s\t", args[j]);
                //     j++;
                // }
                // printf("\n");

                // printf("%s\n", redfile);
                // continue;

                int pathi = check_path(PATHS, path_count, buffer);
                if (pathi == -1) {
                    // printf("Illegal command: %s\n", buffer);
                    // printf("No such command in given path values\n");
                    printf("Invalid command: %s\n", buffer);
                    continue;
                }
                else if (pathi == -2) {
                    strcpy(cmd, buffer);
                } else {
                    temp = (char *)malloc(strlen(PATHS[pathi]));
                    strcpy(temp, PATHS[pathi]);
                    strcat(temp, buffer);
                    strcpy(cmd, temp);
                    free(temp);
                }

                /*fork-exec*/
                pid = fork();
                if (pid == 0) {
                    if (red_flag == IPR) {
                        close(0);
                        open(redfile, O_RDONLY|O_CREAT, S_IRUSR|S_IWUSR);
                    }
                    else if (red_flag == OPR) {
                        close(1);
                        open(redfile, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
                    }
                    else if (red_flag == ORA) {
                        close(1);
                        open(redfile, O_WRONLY|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR);
                    }
                    execv(cmd, args);
                } else {
                    if (strcmp(buffer, "exit") == 0) {
                        status = 0;
                    } 
                    wait(0);
                }
            }
        }
    }
    return 0;
}



int check_path(char **patharr, int path_count, char *cmd) {
    char fullpath[1024];
    int index = -1;
    for (int i = 0; i < path_count; i++) {
        strcpy(fullpath, patharr[i]);
        strcat(fullpath, cmd);
        if (access(fullpath, X_OK) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        if (access(cmd, X_OK) == 0) {
            index = -2;
        }
    }
    return index;
}


void remove_leading_and_trailing_space(char *dest, char *src) {
    int idx = 0, k = 0;

    while (src[idx] == ' ' || src[idx] == '\t') {
        idx++;
    }
    int l = strlen(src);
    for (int i = idx; i < l; i++) {
        dest[k] = src[i];
        k++;
    }
    dest[k] = '\0';
    l = strlen(dest) - 1;
    while (l >= 0 && (dest[l] == '\t' || dest[l] == ' ')) {
        dest[l] = '\0';
        l--;
    }
    return;
}