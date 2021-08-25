
#include "list.h"
#include <fxlib.h>

typedef struct File
{
    list_item_t list_item;

    FONTCHARACTER path[256]; // the widechar path to the file
    char basename[32];       // the printable file basename
} file_t;

/**
 * Get the files from the specified storage area.
 */
void File_ListFiles(list_t *list, char sdCard);

/**
 * Checks if the SD card is available.
 */
int File_IsSDAvailable();
