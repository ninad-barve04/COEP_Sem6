typedef struct dnode{
		int data;
		struct dnode* next;
		struct dnode* prev;
}dnode;

typedef dnode* dll;

// dll init_list( int size );
void init_list(dll* list);
int count_list(dll list);
void traverse(dnode *node);
dnode *search_list( dll list, int dat );
void replace_node(dll *list, int list_data, int replace_data);
void append_list(dll *list, int dat);
void insert_after_data( dll *list, int insert_data, int list_data );
void insert_before_data( dll *list, int insert_data, int list_data );
void insert_at_index( dll *list, int insert_data, int index );
void insert_at_head(dll *list, int insert_data);
void remove_node(dll *list, int dat);
void cleanup_list( dll *list );