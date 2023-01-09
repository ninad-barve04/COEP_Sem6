#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dcll.h"



void append_list(node **head, entry *packet) {
    node *nn = (node *)malloc(sizeof(node));
    nn->mis = packet->mis;
    nn->marks = packet->marks;
    nn->name = (char *)malloc((int)strlen(packet->name) + 1);
    strcpy(nn->name, packet->name);

    if ((*head) == NULL) {
        // printf("Head NULL\n");
        nn->next = nn->prev = nn;
        (*head) = nn;
    } else {
        // printf("Head NOT NULL\n");
        node *last = (*head)->prev;
        nn->next = (*head);
        nn->prev = last;
        last->next = nn;
        (*head)->prev = nn;
    }
    return;
}


void traverse(node *head) {
    node *temp = head;
    while (temp->next != head) {
        printf("%d->", temp->mis);
        temp = temp->next;
    }
    printf("%d->END\n", temp->mis);
    return;
}


void sort_marks(node **head, int mode) {
    if ((*head) == NULL) {
        return;
    }
    int swapped = 1;
    float tempdata = 0.0;
    node *temp = NULL;
    node *last = (*head);
    do {
        swapped = 0;
        temp = *head;

        while (temp->next != last) {
            if (temp->marks > temp->next->marks) {
                tempdata = temp->marks;
                temp->marks = temp->next->marks;
                temp->next->marks = tempdata;
                swapped = 1;
            }
            temp = temp->next;
        }
        last = temp;
        
    } while (swapped);
    
    return;
}

node * search(node *head, int mis) {
    if (head == NULL) {
        return NULL;
    }
    if (head->mis == mis) {
        return head;
    }

    node *temp = head;
    node *last = head->prev;

    while (temp != last) {
        if (temp->mis == mis) {
            return temp;
        }
        temp = temp->next;
    }

    if (temp->next == head) {
        return NULL;
    }
    
}

void insert_sorted(node **head, entry *packet) {
    node *nn = (node *)malloc(sizeof(node));
    nn->mis = packet->mis;
    nn->name = (char *)malloc(sizeof((int)strlen(packet->name)));
    strcpy(nn->name, packet->name);
    nn->marks = packet->marks;
    
    node *temp = (*head);
    while (temp->next != (*head)) {
        if (temp->mis > packet->mis) {

        }



        temp = temp->next;
    }
}


/*
int main(int argc, char const *argv[])
{
    node *root = NULL;
    append_list(&root, 40);
    append_list(&root, 20);
    append_list(&root, 40);
    append_list(&root, 10);
    traverse(root);

    sort(&root, 0);
    traverse(root);
    return 0;
}
*/
