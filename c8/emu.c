#include "emu.h"

#include <stdlib.h>
#include <string.h>

#define V		(emu->memory.reserved.v)
#define I		(emu->memory.reserved.i)
#define ST		(emu->memory.reserved.st)
#define DT		(emu->memory.reserved.dt)
#define PC		(emu->memory.reserved.pc)
#define SP		(emu->memory.reserved.sp)
#define STACK	(emu->memory.reserved.stack)
#define DISPLAY (emu->memory.reserved.display)
#define FONT	(emu->memory.reserved.font)
#define KEYS	(emu->memory.reserved.keys)
#define KEY(k)  ((emu->memory.reserved.keys >> (k)) & 1)

#define PEEK(addr)		(emu->memory.bytes[addr])
#define POKE(addr, val)	(emu->memory.bytes[addr] = (val))

// TODO: fill out
static uint8_t fontData[0x10 * C8_FONT_HEIGHT] = {
	0
};

/**
 * Initialise the virtual machine
 */
void C8_Emu_Init(c8_emu* emu)
{
	int i;
	
	// clear out the registers
	for (i = 0; i < 0x10; i++) V[i] = 0x00;
	I = 0x0000;
	ST = 0x00;
	DT = 0x00;
	for (i = 0; i < C8_STACK_SIZE; i++) STACK[i] = 0x0000;
	KEYS = 0x0000;
	
	// clear out the screen
	for (i = 0; i < C8_DISPLAY_SIZE; i++) DISPLAY[i] = 0x00;
	
	// set the program counter to starting address
	PC = C8_ENTRY_POINT;
	
	// copy in the font data
	memcpy(FONT, fontData, sizeof(fontData));
}

/**
 * Execute the next instruction in the machine
 */
int C8_Emu_Cycle(c8_emu* emu)
{
	
}

/**
 * Tick down the timer (call this 60 times a second)
 */
void C8_Emu_Tick(c8_emu* emu)
{
	if (ST > 0) ST--;
	if (DT > 0) DT--;
}
