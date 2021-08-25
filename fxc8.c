
#include "fxlib.h"
#include "c8/emu.h"

c8_emu emu;

/**
 * View to select files and change emulator settings.
 */
static void fileSelect()
{
	unsigned int key;
	int i;
	FONTCHARACTER pathname[] = {'\\', '\\', 'c', 'r', 'd', '0', '\\', '*', 0};
	FONTCHARACTER foundfile[32];
	char filename[32];
	int findhandle;
	FILE_INFO fileinfo;

	Bdisp_AllClr_DDVRAM();

	locate(1, 1);
	Print((unsigned char *)"Chip-8 Programs");

	// get the first few files on the SD card
	if (Bfile_FindFirst(pathname, &findhandle, foundfile, &fileinfo))
	{
		locate(1, 2);
		Print("Not found. ;(");
	}

	// print out four file names
	for (i = 0; i < 6; i++)
	{
		fontCharToChar(foundfile, filename);
		locate(2, 2 + i);
		Print(filename);

		if (Bfile_FindNext(findhandle, foundfile, &fileinfo))
			break;
	}
	Bfile_FindClose(findhandle);

	while (1)
	{
		GetKey(&key);
	}
}

/**
 * Program entry point.
 */
int AddIn_main(int isAppli, unsigned short OptionNum)
{
	fileSelect();

	return 1;
}

// don't change the code below

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section

#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
	return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section
