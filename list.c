#include "list.h"

#include <stdlib.h>

void List_Init(list_t* list, size_t size) {
	list->size = size;
	list->count = 0;
	list->items = NULL;
}

void List_Destroy(list_t* list) {
	list->size = 0;
	list->count = 0;
	free(list->items);
}

int List_Push(list_t* list, void* item) {
	list->count++;

	if (list->count == 1) {
		list->items = malloc(list->count * list->size);
		if (list->items == NULL) goto fail;
		goto write;
	}

	list->items = realloc(list->items, list->count * list->size);
	if (list->items == NULL) goto fail;

write:
	memcpy((char*) List_End(list) - list->size, item, list->size);
	return 0;

fail:
	return 1;
}

void List_Clear(list_t* list) {
	list->count = 0;
	free(list->items);
}

void* List_Begin(list_t* list) {
	return list->items;
}

void* List_End(list_t* list) {
	return (char*)(list->items) + (list->count * list->size);
}
