#include <string.h>

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
    char *str = "  -123";
    int a = atoi(str);
    printf("%d\n", a);
    return 0;
}
