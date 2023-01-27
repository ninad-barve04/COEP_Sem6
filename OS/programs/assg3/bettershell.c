/**
 * @file shell.c
 * @author Ninad Barve 
 * @brief OS Course Assignment
 * @date 2023-01-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */


//USE FUNCTIONS AS MUCH AS POSSIBLE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

/*Color, Bold and Reset characters*/
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define RESET     "\x1b[0m"
#define BOLD      "\x1B[1m" 

#define NOR 0 /*No Redirection */
#define IPR 1 /*Input Redirection*/
#define OPR 2 /*Output Redirection*/
#define ORA 4 /*Output Redirection with Append*/

#define BUFFERSIZE 1024
#define MICROBUFFER 16

/*Utility function declarations*/
int check_path(char **patharr, int path_count, char *cmd);
void remove_leading_and_trailing_space(char *dest, char *src);
void execute_pipe(char *input, char *PATHS[], int path_count);


int main()
{
    /*various char arrays to be used to store different strings*/
    char buffer[BUFFERSIZE], cwd[BUFFERSIZE], prompt[BUFFERSIZE], path[BUFFERSIZE], redfile[BUFFERSIZE], cmd[BUFFERSIZE];
    char *args[BUFFERSIZE]; /*argument array. All commands get split at whitespace*/
    char *history[MICROBUFFER];
    char *temp; /*char pointer for all temporary storing purposes*/
    int pid; /*process id of child and parent*/
    int custom = 0; /*custom prompt flag*/
    int len;
    int status = 1; /*staus flag for exiting out of loop*/
    int red_flag = NOR; /*redirection flag*/
    
    char *PATHS[64], PATH[BUFFERSIZE];
    PATHS[0]="/usr/bin/";
    int path_count = 1;
    
    struct stat stats;

    /*Warning messages for some features*/
    printf("%s%sWARNING%s: Default path set to /usr/bin. Adding new paths via PATH=newpath1:newpath2:... will delete existing paths\n", C_RED, BOLD, RESET);

    while (status) {
        /*for displaying the prompt if it is the cwd or a custom one*/
        if (custom == 0) {
            getcwd(cwd, BUFFERSIZE);
            strcpy(prompt, cwd);
        }    
        printf("%s%s%s%s$ ", C_GREEN, BOLD, prompt, RESET);
        
        /*Getting the user command shell*/
        if (fgets(buffer, BUFFERSIZE, stdin) == NULL) {
            /*checking for ctrl-D*/
            printf("%s%s\nExitting Gracefully!!%s\n", C_CYAN, BOLD, RESET);
            status = 0;
            break;
        }
        buffer[strlen(buffer)-1] = '\0';

        /*removing leading and trailing whitespaces*/
        temp = (char *)malloc(strlen(buffer));
        strcpy(temp, buffer);
        remove_leading_and_trailing_space(buffer, temp);
        free(temp);

        for (int m = 0; m < MICROBUFFER; m++) {

        }
        
        /*Following is the set of if, else if and else conditions handling the shell*/

        /*Setting the custom prompt*/
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
            printf("PS1=\"%s\n",temp);
            continue;
        }
        /*Gracefully exiting the shell*/
        else if (strcmp(buffer, "exit") == 0) {
            printf("%s%sExitting Gracefully!!%s\n", C_CYAN, BOLD, RESET);
            status = 0;
            break;
        }
        /*Handling changing directory cd*/
        else if (strstr(buffer, "cd") != NULL) {
            temp = &(buffer[3]);
            strcpy(path, temp);
            if (strlen(path) == 0) {
                strcpy(path, getenv("HOME"));
                // strcpy(path, "/home/ninad");
            }
            int c = chdir(path);
            if (c < 0) {
                if (errno == ENOENT) {
                    printf("%s%sError%s: The directory specified in path does not exist.\n", C_RED, BOLD, RESET);
                }
                else if (errno == ENOTDIR) {
                    printf("%s%sError%s: A component of path is not a directory\n", C_RED, BOLD, RESET);
                }
                else if (errno == EACCES) {
                    printf("%s%sError%s: Search permission was denied for one of the components of path./n", C_RED, BOLD, RESET);
                }
            }
            continue;
        } 
        /*Handling changing the path PATH=newpaths*/
        else if (strstr(buffer, "PATH=") != NULL) {
            temp = &(buffer[5]);
            strcpy(PATH, temp);
            path_count = 0;
            char *p = strtok(PATH, ":");
            while (p != NULL) {
                stat(p, &stats);
                if (S_ISDIR(stats.st_mode)) {
                    PATHS[path_count++] = p;
                } else {
                    printf("%s%sError%s: %s is not a valid path to a directory\n", C_RED, BOLD, RESET, p);
                }
                p = strtok(NULL, ":");
            }
            printf("PATH set to ");
            for (int k = 0; k < path_count; k++) {
                printf("%s\t", PATHS[k]);
            }
            printf("\n");
            continue;
        }
        /*Command to see the path variable for this shell program*/
        else if (strcmp(buffer, "GETPATH") == 0) {
            printf("PATH set to ");
            for (int k = 0; k < path_count; k++) {
                printf("%s\t", PATHS[k]);
            }
            printf("\n");
        }
        else if (strcmp(buffer, "history") == 0) {
             
        }
        /*No special commands. Proceed to normal execution*/
        else {
            /*checking existence of any paths*/
            if (path_count == 0) {
                printf("%s%sError%s:No value in PATH\n", C_RED, BOLD, RESET);
                printf("Please set PATH variable\n");
                printf("Syntax: PATH=path1:path2:...\n");
                continue;
            }
            /*adding PATH to the command*/
            else {
                /*Checking presence of pipe character*/
                if (strstr(buffer, "|") != NULL) {
                    /*Pipe handler function call*/
                    printf("Executing pipe\n");
                    execute_pipe(buffer, PATHS, path_count);
                    continue;
                }
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
                        strcpy(redfile, p);
                        break;
                    }
                }
                args[i] = NULL;


                /*Checking path is command exists*/
                int path_idx = check_path(PATHS, path_count, buffer);
                if (path_idx == -1) {
                    printf("%s%sError%s: Invalid command: %s\n", C_RED, BOLD, RESET, buffer);
                    continue;
                }
                /*User has provided complete path to command*/
                else if (path_idx == -2) {
                    strcpy(cmd, buffer);
                } else {
                    temp = (char *)malloc(strlen(PATHS[path_idx]));
                    strcpy(temp, PATHS[path_idx]);
                    len = strlen(temp);
                    if (temp[len-1] != '/') {
                        strcat(temp, "/");
                    }
                    strcat(temp, buffer);
                    strcpy(cmd, temp);
                    free(temp);
                }

                /*fork-exec*/
                pid = fork(); /*The child creator*/
                if (pid == 0) {
                    /*Checking redirection flags*/
                    /*Input redirection*/
                    if (red_flag == IPR) {
                        /*For input redirection close stdin and open specified file*/
                        close(0);
                        if (access(redfile, R_OK) == 0) {
                            open(redfile, O_RDONLY, S_IRUSR|S_IWUSR);
                        } else {
                            printf("%s%sError%s:Input file does not exist.\n", C_RED, BOLD, RESET);
                            continue;
                        }
                    }
                    /*Output redirection*/
                    else if (red_flag == OPR) {
                        /*Close stdout and open specified file*/
                        close(1);
                        open(redfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
                    }
                    else if (red_flag == ORA) {
                        /*Close stdout and open specified file*/
                        close(1);
                        open(redfile, O_WRONLY|O_APPEND|O_CREAT, S_IRUSR|S_IWUSR);
                    }
                    /*exec syscall. The one executing all the normal commands of the shell*/
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

/*The utility function definitions*/
/**
 * @brief Find the correct path of executables
 *        Do this by iterating through PATH array and check if executable
 *        is present.
 * 
 * @param patharr       All paths present in array as specified by user
 * @param path_count    no of paths present in list
 * @param cmd           Name of command
 * @return int          Index of the correct path where the executable is found
 *                      val = -1 means command not present in all PATHS
 *                      val = -2 means whatever user typed is a command
 */
int check_path(char **patharr, int path_count, char *cmd) {
    char fullpath[BUFFERSIZE];
    int index = -1, len;
    for (int i = 0; i < path_count; i++) {
        strcpy(fullpath, patharr[i]);
        len = strlen(fullpath);
        if (fullpath[len-1] != '/') {
            strcat(fullpath, "/");
        }
        strcat(fullpath, cmd);
        if (access(fullpath, X_OK) == 0) {
            index = i;
            break;
        }
    }
    /*If index is still -1, there might not be an executable present
     *or the user might have specified the path in the command
     *So checking if the command exists*/
    if (index == -1) {
        if (access(cmd, X_OK) == 0) {
            index = -2;
        }
    }
    return index;
}

/**
 * @brief Removes leading and trailing whitespace characters
 *        They are ' ' and '\t'
 * 
 * @param dest Destination string with whitespace characters removed
 *             this should not be malloced
 * @param src  Original string from input
 */
void remove_leading_and_trailing_space(char *dest, char *src) {
    int idx = 0, k = 0;
    dest = (char *)malloc(strlen(src));
    while (src[idx] == ' ' || src[idx] == '\t') {
        idx++;
    }
    int l = strlen(src);
    /*Removing trailing characters*/
    for (int i = idx; i < l; i++) {
        dest[k] = src[i];
        k++;
    }
    dest[k] = '\0';
    l = strlen(dest) - 1;
    /*Removing leading characters*/
    while (l >= 0 && (dest[l] == '\t' || dest[l] == ' ')) {
        dest[l] = '\0';
        l--;
    }
    return;
}

void execute_pipe(char *input, char *PATHS[], int path_count) {
    /*If we are inside here, it means we already have a presence pf pipe here*/
    /*strtok modifies the string so copying input to a separate buffer*/
    char *buffer = strdup(input);
    char *args[BUFFERSIZE];
    char redfile[BUFFERSIZE], cmd[BUFFERSIZE];
    char *temp;
    int red_flag = NOR, len = 0;
    int pid;
    /*count no of commands*/
    int count = 0, i = 0;
    while (buffer[i] != '\0') {
        if (buffer[i] == '|') {
            count++;
        }
        i++;
    }
    count++;
    // printf("Count %d\n", count);
    int pfdarray[count - 1][2];
    for (int k = 0; k < count-1; k++) {
        pipe(pfdarray[k]);
    }
    char **cmd_array = (char **)malloc(count);
    for (int m = 0; m < count; m++) {
        /*make null*/
    }
    char *p = strtok(buffer, "|");
    i = 0;
    while (p != NULL) {
        // remove_leading_and_trailing_space(cmd_array[i], p);
        cmd_array[i] = p;
        i++;
        p = strtok(NULL, "|");
    }

    for (i = 0; i < count; i++) {
        // printf("cmd_Array[i]: %s\n", cmd_array[i]);
        /**
         * in first iteration i = 0;
         *      close(1) write closed
         *      dup(pipe ka 0 ka 1)
         * in all middle iterations
         *      curr itr = x
         *      close(0)
         *      dup(pipe array ke x-1 ka 0)
         *      close(1)
         *      dup(pipe array ke x ka 1)
         * in last iteration
         *      close(0)
         *      dup(pipe array ke x-1 ka 0)
         */
        /*Checking for redirection*/


        if (strstr(cmd_array[i], "<") != NULL) {
            red_flag = IPR;
        } else if (strstr(cmd_array[i], ">") != NULL) {
            if (strstr(cmd_array[i], ">>") != NULL) {
                red_flag = ORA;
            } else {
                red_flag = OPR;
            }
        } else {
            red_flag = NOR;
        }


        /*Now creating an argument array for handling options*/
        p = strtok(cmd_array[i], " ");
        int k = 0, idx;
        while (p != NULL) {
            args[k++] = p;
            p = strtok(NULL, " ");
            if (p != NULL && (strcmp(p, ">") == 0 || strcmp(p, ">>") == 0 || strcmp(p, "<") == 0)) {
                p = strtok(NULL, " ");
                strcpy(redfile, p);
                break;
            }
        }
        args[k] = NULL;

        /*Checking path is command exists*/
        int path_idx = check_path(PATHS, path_count, cmd_array[i]);
        if (path_idx == -1) {
            printf("%s%sError%s: Invalid command: %s\n", C_RED, BOLD, RESET, cmd_array[i]);
            continue;
        }
        /*User has provided complete path to command*/
        else if (path_idx == -2) {
            strcpy(cmd, cmd_array[i]);
        } else {
            temp = (char *)malloc(strlen(PATHS[path_idx]));
            strcpy(temp, PATHS[path_idx]);
            len = strlen(temp);
            if (temp[len-1] != '/') {
                strcat(temp, "/");
            }
            strcat(temp, cmd_array[i]);
            strcpy(cmd, temp);
            free(temp);
        }
        // printf("%d cmd: %s\n", i, cmd);
        pid = fork();
        if (pid == 0) {
            printf("%d cmd inside fork(): %s\n", i, cmd);
            if (i == 0) {
                close(1);
                dup(pfdarray[i][1]);
            }
            else if (i == count - 1) {
                close(0);
                dup(pfdarray[i-1][0]);
                // printf("In last command\n%s\n", cmd);
            } else {
                close(0);
                dup(pfdarray[i-1][0]);
                close(1);
                dup(pfdarray[i][1]);
            }

            for (int m = 0; m < count; m++) {
                close(pfdarray[m][0]);
                close(pfdarray[m][1]);
            }
            
            execv(cmd, args);
        } else {
            // for (int m = 0; m < count; m++) {
                printf("Waiting for %d, cmd = %s\n", pid, cmd);
                wait(0);
            // }
        }

    }



    free(buffer);
}