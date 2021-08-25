#ifndef _FXC8_LIST_H
#define _FXC8_LIST_H

#include <stdlib.h>

typedef struct List list_t;
typedef struct ListItem list_item_t;

struct List {
	list_item_t* head;
	list_item_t* tail;
};

struct ListItem {
	list_t* list;
	list_t* next;
	list_t* prev;
};

void List_Clear(list_t*);
void List_Push(list_t*);

#endif
