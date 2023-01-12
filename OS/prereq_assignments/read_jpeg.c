#include <stdio.h>

int main(int argc, char const *argv[])
{
    FILE *fimg = fopen("Cat03.jpg", "r");
    int count = 0;
    unsigned char curr, prev;
    
    while ((curr = fgetc(fimg)) != EOF) {
        printf("%x ", curr);
        count++;
        if (count == 80) {
            printf("\n");
            count = 0;
        }
        if (feof(fimg)) {
            printf("\nEnd of file reached\n");
            break;
        }

        if (prev == '0xff' && curr == '0xd8') {
            printf("\nEnd of header\n");
            break;
        }


        prev = curr;
    }

    fclose(fimg);

    return 0;
}

