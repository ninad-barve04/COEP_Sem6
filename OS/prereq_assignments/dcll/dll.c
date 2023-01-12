#include <stdio.h>
#include <stdlib.h>

#include "dll.h"


void init_list(dcll* list) {
    *list = NULL;
    return;
}

/*  function appends a node at the end with data = dat */

void append_list(dcll *list, int dat){
    dnode *nn = (dnode *)malloc(sizeof(dnode)); 
    nn->data =dat;

    if( *list == NULL ) { 
        nn->next = NULL;
        nn->prev = NULL;
        *list = nn; 
    } else {
        dnode *head= *list;
        while(head->next != NULL) {
            head = head->next;
        }

        head->next= nn; 
        nn->prev= head;
    }
 
}


/* function inserts a node after a node with data = list_data
 * inserted node has data = insert_data */

void insert_after_data( dcll *list, int insert_data, int list_data ) {
    dnode *node = search_list(*list, list_data);
    if (node == NULL) {
        printf("node with data %d not found\n",list_data);
        return;
    }
    dnode *newnode = (dnode *)malloc(sizeof(dnode));
    newnode->data = insert_data;
    newnode->prev = node;
    if (node->next == NULL) {
        newnode->next = NULL;
        node->next = newnode;
    } else {
        dnode *p = node->next;

        newnode->next = p;
        node->next = newnode;
        p->prev = newnode;
        return;
    }
}


/* function inserts a node before node containing data = list_data */

void insert_before_data( dcll *list, int insert_data, int list_data ) {
    dnode *node = search_list(*list, list_data);
    if (node == NULL) {
        return;
    }
    
    dnode *newnode = (dnode *)malloc(sizeof(dnode));
    newnode->data = insert_data;
    newnode->next = node;
    
    if (node->prev == NULL) {
        newnode->prev = NULL;
        *list = newnode;
    } else {
            dnode *p = node->prev;
            newnode->prev = p;
        node->prev = newnode;
        p->next = newnode;
            return;

    }
}


/* inserts new node at head, i.e. topmost position in list, 
 * with data = insert_data */

void insert_at_head( dcll *list, int insert_data ) {
    dnode *head = *list;

    dnode *newhead = (dnode *)malloc(sizeof(dnode));
    newhead->data = insert_data;
    newhead->prev = NULL;
    newhead->next = head;

    head->prev = newhead;
    *list = newhead;

}


/* inserts new node at specified index */

void insert_at_index( dcll *list, int insert_data, int index ) {
    int count = count_list(*list);
    if (count < index) {
        printf("Index out of bounds!\nRequested index is greater than length of list.\nFunction aborted!");
        return;
    } else if (count == index + 1) {
        append_list(list, insert_data);
    } else if ( index == 0 ) {
        /*  index 0 is head , hence call insert_at_head */
        insert_at_head( list, insert_data );
    } else {
        dnode *p = *list;
        for (int i = 1; i < index; i++) {
            p = p->next;
        }
        dnode *q = p->next;
        dnode *newnode = (dnode *)malloc(sizeof(dnode));
        
        newnode->data = insert_data;
        newnode->next = q;
        newnode->prev = p;

        p->next = newnode;
        q->prev = newnode;
    }
    
    return;
}


/*  function returns number of nodes in the list */

int count_list(dcll list) {
    int count = 0;
    dnode *p = list;
    if (list != NULL) {
        do {
            count = count + 1;
            p = p->next;
        } while (p != NULL);
    }
    return count;
}


/*  function traversed the list and prints the data at each node */

void traverse(dnode *node){
    dnode *p = node;
    if( node == NULL ){
        printf("List is empty\n");
        return;
    }   
    do{ 
        printf("%d  ", p->data);
        p = p->next;
                    
    }while( p != NULL );
    printf("\n\n");
}


/* function searches the list and returns node containing the data 
 * return NULL if data not found*/

dnode *search_list(dcll list, int dat) {
        dnode *head = list;
        while ( head != NULL ) { 
                if (head->data == dat) 
                        break;
                head = head->next;
        }    
        return head;
}


/* replaces a node with data = list_data with data = replace_data */

void replace_node(dcll *list, int list_data, int replace_data) {
    dnode *node = *list;
    dnode *p = search_list(node, list_data);

    p->data = replace_data;
    return;
}

/*  function appends a node at the end with data = dat */

void append_list(dcll *list, int dat){
    dnode *nn = (dnode *)malloc(sizeof(dnode)); 
    nn->data =dat;

    if( *list == NULL ) { 
        nn->next = NULL;
        nn->prev = NULL;
        *list = nn; 
    } else {
        dnode *head= *list;
        while(head->next != NULL) {
            head = head->next;
        }

        head->next= nn; 
        nn->prev= head;
    }
 
}


/* function inserts a node after a node with data = list_data
 * inserted node has data = insert_data */

void insert_after_data( dcll *list, int insert_data, int list_data ) {
    dnode *node = search_list(*list, list_data);
    if (node == NULL) {
        printf("node with data %d not found\n",list_data);
        return;
    }
    dnode *newnode = (dnode *)malloc(sizeof(dnode));
    newnode->data = insert_data;
    newnode->prev = node;
    if (node->next == NULL) {
        newnode->next = NULL;
        node->next = newnode;
    } else {
        dnode *p = node->next;

        newnode->next = p;
        node->next = newnode;
        p->prev = newnode;
        return;
    }
}


/* function inserts a node before node containing data = list_data */

void insert_before_data( dcll *list, int insert_data, int list_data ) {
    dnode *node = search_list(*list, list_data);
    if (node == NULL) {
        return;
    }
    
    dnode *newnode = (dnode *)malloc(sizeof(dnode));
    newnode->data = insert_data;
    newnode->next = node;
    
    if (node->prev == NULL) {
        newnode->prev = NULL;
        *list = newnode;
    } else {
            dnode *p = node->prev;
            newnode->prev = p;
        node->prev = newnode;
        p->next = newnode;
            return;

    }
}


/* inserts new node at head, i.e. topmost position in list, 
 * with data = insert_data */

void insert_at_head( dcll *list, int insert_data ) {
    dnode *head = *list;

    dnode *newhead = (dnode *)malloc(sizeof(dnode));
    newhead->data = insert_data;
    newhead->prev = NULL;
    newhead->next = head;

    head->prev = newhead;
    *list = newhead;

}


/* inserts new node at specified index */

void insert_at_index( dcll *list, int insert_data, int index ) {
    int count = count_list(*list);
    if (count < index) {
        printf("Index out of bounds!\nRequested index is greater than length of list.\nFunction aborted!");
        return;
    } else if (count == index + 1) {
        append_list(list, insert_data);
    } else if ( index == 0 ) {
        /*  index 0 is head , hence call insert_at_head */
        insert_at_head( list, insert_data );
    } else {
        dnode *p = *list;
        for (int i = 1; i < index; i++) {
            p = p->next;
        }
        dnode *q = p->next;
        dnode *newnode = (dnode *)malloc(sizeof(dnode));
        
        newnode->data = insert_data;
        newnode->next = q;
        newnode->prev = p;

        p->next = newnode;
        q->prev = newnode;
    }
    
    return;
}


/* removes node containing data = dat */

void remove_node(dcll *list, int dat) {
    dnode *node = search_list(*list, dat);
    if( node != NULL ){
        dnode *p = node->prev;
        dnode *q = node->next;

        if( p != NULL ){
            p->next = q;
        }
        else {
            *list = q;
        }
        q->prev = p;
        free(node);
    }  
    else{
        printf(" node with data %d not found in list\n", dat);
    }
    return;
    
}


/*  function to cleanup all nodes in a list */

void cleanup_list( dcll *list ) {
    dnode *p = *list;
        if( p == NULL ){
                return;
        }   
        do{ 
        dnode *next_node= p->next;
        free( p );
                p = next_node;
        }while( p != NULL );
    *list = NULL;
}