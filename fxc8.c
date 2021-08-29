
#include <stdio.h>
#include <fxlib.h>
#include <filebios.h>
#include <timer.h>

#include "c8/emu.h"

static c8_emu emu;
static int romFileHandle = -1;
static void (*mode)();

static const FONTCHARACTER testPath[] = { '\\', '\\', 'c', 'r', 'd', '0', '\\', 'T', 'R', 'O', 'N', '.', 'c', 'h', '8', 0 };

void fileSelect();
void loader();
void emulator();

void emulator_debugFrame();
void emulator_drawFrame();
void emulator_cycle();
void emulator_timer();

int emu_key(unsigned int key, unsigned int key2);

/**
 * View to select files and change emulator settings.
 */
static void fileSelect()
{
	unsigned int key;
	char errCode[64] = { 0 };

	Bdisp_AllClr_DDVRAM();

	locate(1, 1);
	Print((unsigned char *)"Chip-8 Programs");

	romFileHandle = Bfile_OpenFile(testPath, _OPENMODE_READ_SHARE);
	if (romFileHandle < 0) {
		sprintf(errCode, "Failed file open: %d\n", romFileHandle);
		Print(errCode);
		while (1) GetKey(&key);
	}

	mode = loader;
}

/**
 * Loads the ROM into the emulator RAM
 */
static void loader() {
	int ret;
	char errCode[64] = { 0 };

	// init the emulator
	locate(1, 1);
	Print("Init Chip-8 emulator");
	C8_Emu_Init(&emu);

	// load the program into memory
	locate(1, 2);
	Print("Loading program");
	ret = Bfile_ReadFile(romFileHandle, &emu.memory.bytes[C8_ENTRY_POINT], C8_MEM_SIZE - C8_ENTRY_POINT, 0);
	if (ret < 0) {
	locate(1, 3);
		sprintf(errCode, "Failed program load: %d", ret);
		Print(errCode);
	}

	// close file and switch mode
	Bfile_CloseFile(romFileHandle);
	romFileHandle = -1;
	mode = emulator;
}

static void emulator() {
	unsigned int key, key2, unused;
	int emuKey;
	int ret;
	char buffer[16];

	Bdisp_AllClr_DDVRAM();

	// set timers for screen draw, CPU and ticks
	// SetTimer(ID_USER_TIMER1, 500, emulator_drawFrame);
	SetTimer(ID_USER_TIMER2, 16, emulator_cycle);
	// SetTimer(ID_USER_TIMER3, 500, emulator_timer);

	while (1)
	{
		key, key2 = 0;
		ret = Bkey_GetKeyWait(&key, &key2, KEYWAIT_HALTON_TIMERON, 0, 0, &unused);
		if (ret == KEYREP_KEYEVENT) {
			if (key == KEY_CTRL_AC) {
				goto exit;
			}

			// key = -1;
		}
		emuKey = emu_key(key, key2);

		emulator_debugFrame();

		locate(21 - 8, 8);
		sprintf(buffer, "%8X", emuKey);
		Print(buffer);
		Bdisp_PutDisp_DD();
	}

exit:
	// KillTimer(ID_USER_TIMER1);
	KillTimer(ID_USER_TIMER2);
	// KillTimer(ID_USER_TIMER3);

	mode = fileSelect;
}

///

void emulator_debugFrame() {
	char buffer[64] = {0};
	int i;

	for (i = 0; i < 16; i++) {
		locate(1 + 5 * (i / 8), 1 + (i % 8));

		sprintf(buffer, "V%X", i);
		PrintRev(buffer);
		sprintf(buffer, "%02X", emu.memory.reserved.v[i]);
		Print(buffer);
	}

	locate(11, 1);
	PrintRev(" I");
	sprintf(buffer, "%04X", emu.memory.reserved.i);
	Print(buffer);

	locate(11, 2);
	PrintRev("ST");
	sprintf(buffer, "  %02X", emu.memory.reserved.st);
	Print(buffer);

	locate(11, 3);
	PrintRev("DT");
	sprintf(buffer, "  %02X", emu.memory.reserved.dt);
	Print(buffer);

	locate(11, 4);
	PrintRev("PC");
	sprintf(buffer, "%04X", emu.memory.reserved.pc);
	Print(buffer);

	locate(11, 5);
	PrintRev("SP");
	sprintf(buffer, "  %02X", emu.memory.reserved.sp);
	Print(buffer);

	locate(11, 6);
	PrintRev(" K");
	sprintf(buffer, "%04X", emu.memory.reserved.keys);
	Print(buffer);

	Bdisp_PutDisp_DD();
}

void emulator_drawFrame() {
}

void emulator_cycle() {
	C8_Emu_Cycle(&emu);
}

void emulator_timer() {
	C8_Emu_Tick(&emu);
}

int emu_key(unsigned int x, unsigned int y) {
	switch (y) {
	case 5:
		switch (x) {
		case 7: return 1;
		case 6: return 2;
		case 5: return 3;
		case 4: return 0xC;
		default: return -1;
		}
		break;

	case 4:
		switch (x) {
		case 7: return 4;
		case 6: return 5;
		case 5: return 6;
		case 4: return 0xD;
		default: return -1;
		}
		break;

	case 3:
		switch (x) {
		case 7: return 7;
		case 6: return 8;
		case 5: return 9;
		case 4: return 0xE;
		default: return -1;
		}
		break;

	case 2:
		switch (x) {
		case 7: return 0xA;
		case 6: return 0;
		case 5: return 0xB;
		case 4: return 0xF;
		default: return -1;
		}
		break;

	default: return -1;
	}
}

///

/**
 * Program entry point.
 */
int AddIn_main(int isAppli, unsigned short OptionNum)
{
	mode = fileSelect;

	while (1) {
		mode();
	}

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
