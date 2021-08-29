#include <file.h>

#include "list.h"
#include <fxlib.h>
#include <filebios.h>

static const FONTCHARACTER FLASH_NAME[] = { '\\', '\\', 'f', 'l', 's', '0', '\\', '*', 0 };
static const FONTCHARACTER SD_CARD_NAME[] = { '\\', '\\', 'c', 'r', 'd', '0', '\\', '*', 0 };

void File_ListFiles(list_t *list, char sdCard) {
	List_Clear(list);

}

