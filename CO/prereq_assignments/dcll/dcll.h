#define ASC 1
#define DEC 2

typedef struct entry {
    int mis;
    char *name;
    float marks;
} entry;

typedef struct node {
    int mis;
    char *name;
    float marks;
    struct node *next;
    struct node *prev;
} node;

typedef node *DCLL;

void append_list(node **head, entry *packet);
void traverse(node *head);
void sort_marks(node **head, int mode);
node *search(node *head, int mis);
