#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // int pfdarray[1][2];
    // char *cmd = "ls|wc";
    // int count = 2;
    // int pid = fork();
    // if (pid == 0) {
    //     // printf("%d cmd inside fork(): %s\n", i, cmd);
    //     if (i == 0) {
    //         close(1);
    //         dup(pfdarray[i][1]);
    //         close(pfdarray[i][0]);
    //     }
    //     else if (i == count - 1) {
    //         close(0);
    //         dup(pfdarray[i-1][0]);
    //         // printf("In last command\n%s\n", cmd);
    //     } else {
    //         close(0);
    //         dup(pfdarray[i-1][0]);
    //         close(1);
    //         dup(pfdarray[i][1]);
    //     }

    //     // for (int m = 0; m < count; m++) {
    //     //     close(pfdarray[m][0]);
    //     //     close(pfdarray[m][1]);
    //     // }
        
    //     execv(cmd, args);
    // } else {
    //     wait(0);
    // }
    // char *str = "  -123";
    // int a = atoi(str);
    // printf("%d\n", a);
    
    typedef struct JobList {
        int pid;
        char *jobname;
        struct JobList *next;
    } JobList;

    JobList *joblist = NULL;
    
    JobList *nn = (JobList *)malloc(sizeof(JobList));
    nn->jobname = strdup("hello");
    nn->pid = 20;
    nn->next = NULL;
    JobList *temp = joblist;
    if (temp == NULL) {
        joblist = nn;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        } 
        temp->next = nn;
    }

    nn = (JobList *)malloc(sizeof(JobList));
    nn->jobname = strdup("what");
    nn->pid = 10;
    nn->next = NULL;
    temp = joblist;
    if (temp == NULL) {
        joblist = nn;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        } 
        temp->next = nn;
    }

    nn = (JobList *)malloc(sizeof(JobList));
    nn->jobname = strdup("there there");
    nn->pid = 33;
    nn->next = NULL;
    temp = joblist;
    if (temp == NULL) {
        joblist = nn;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        } 
        temp->next = nn;
    }

    nn = (JobList *)malloc(sizeof(JobList));
    nn->jobname = strdup("ooooooo");
    nn->pid = 69;
    nn->next = NULL;
    temp = joblist;
    if (temp == NULL) {
        joblist = nn;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        } 
        temp->next = nn;
    }


    temp = joblist;
    int jc = 1;
    if (temp == NULL) {
        printf("No pending jobs!!\n");
    } else {
        while (temp != NULL) {
            printf("%d\t%d\t%s\n", jc, temp->pid, temp->jobname);
            temp = temp -> next;
            jc++;
        }
    }
    printf("Now deleting\n");

    int i = 1, index = 3;
    temp = joblist;
    JobList *p = temp;
    while (i < index) {
        p = temp;
        temp = temp->next;
        i++;
    }
    p->next = temp->next;
    temp->next = NULL;


    temp = joblist;
    jc = 1;
    if (temp == NULL) {
        printf("No pending jobs!!\n");
    } else {
        while (temp != NULL) {
            printf("%d\t%d\t%s\n", jc, temp->pid, temp->jobname);
            temp = temp -> next;
            jc++;
        }
    }
    
    return 0;
}
