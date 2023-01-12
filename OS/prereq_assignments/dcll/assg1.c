#include <stdio.h>
#include <string.h>

#include "dcll.h"



int main(int argc, char const *argv[])
{
    FILE *f = fopen(argv[1], "r");

    DCLL root = NULL;

    char line[50];
    char *token;
    fgets(line, 50, f);
    
    entry packet;
    while (fgets(line, 50, f)) {
        packet.name = strtok(line, ",");
        char *mis = strtok(NULL, ",");
        char *marks = strtok(NULL, ",");
        sscanf(mis, "%d", &(packet.mis));
        sscanf(marks, "%f", &(packet.marks));
        append_list(&root, &packet);
    }
    traverse(root);
    
    // node *random = search(root, 112003016);
    // printf("%d\t%s\t%f\n", random->mis, random->name, random->marks);

    return 0;
}
