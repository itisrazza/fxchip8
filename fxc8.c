/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
#include "fxlib.h"

void fontCharToChar(FONTCHARACTER* fca, char* charArray);

/**
 * View to select files and change emulator settings.
 */
static void fileSelect() {
	unsigned int key;
	int i;
	FONTCHARACTER pathname[] = { '\\', '\\', 'c', 'r', 'd', '0', '\\', '*', 0 };
	FONTCHARACTER foundfile[32];
	char filename[32];
	int findhandle;
	FILE_INFO fileinfo;

    Bdisp_AllClr_DDVRAM();

    locate(1, 1);
    Print((unsigned char*)"Chip-8 Programs");
	
	// get the first few files on the SD card
	if (Bfile_FindFirst(pathname, &findhandle, foundfile, &fileinfo)) {
		locate(1, 2);
		Print("Not found. ;(");
	}
	
	// print out four file names
	for (i = 0; i < 6; i++) {
		fontCharToChar(foundfile, filename);
		locate(2, 2 + i);
		Print(filename);
		
		if (Bfile_FindNext(findhandle, foundfile, &fileinfo)) break;
	}
	Bfile_FindClose(findhandle);
	
    while(1){
        GetKey(&key);
    }
}

//****************************************************************************
//  AddIn_main (Sample program main function)
//
//  param   :   isAppli   : 1 = This application is launched by MAIN MENU.
//                        : 0 = This application is launched by a strip in eACT application.
//
//              OptionNum : Strip number (0~3)
//                         (This parameter is only used when isAppli parameter is 0.)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int AddIn_main(int isAppli, unsigned short OptionNum)
{
	fileSelect();

    return 1;
}

void fontCharToChar(FONTCHARACTER* fca, char* charArray) {
	int i = 0;
	
	while (*fca != 0x0000) {
		if (
			*fca >> 8 == 0x7F ||
			*fca >> 8 == 0xF9 || 
			*fca >> 8 == 0xE5 || 
			*fca >> 8 == 0xE6
		) {
			*(charArray++) = *fca >> 8;
		}
		*(charArray++) = *(fca++);
	}
	charArray[i] = 0;
}



//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

