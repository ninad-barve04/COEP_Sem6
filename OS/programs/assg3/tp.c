#include <string.h>

int main(int argc, char const *argv[])
{
    int a[4][2];
    char s[] = "lalala";
    char *p = strtok(s, "|");
    while (p != NULL) {
        printf("%s\n", p);
        p = strtok(NULL, "|");
    }
    return 0;
}
