#ifndef _FXC8_LIST_H
#define _FXC8_LIST_H

#include <stdlib.h>

typedef struct List list_t;

struct List {
	size_t size;
	size_t count;

	void* items;
};

void List_Init(list_t* list, size_t size);
void List_Destroy(list_t* list);

int List_Push(list_t* list, void* item);
void List_Clear(list_t* list);

void* List_Begin(list_t* list);
void* List_End(list_t* list);

#endif
