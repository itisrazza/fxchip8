#ifndef _FXC8_C8_EMU_H
#define _FXC8_C8_EMU_H

#include <stdlib.h>

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

#define C8_MEM_SIZE   0x1000
#define C8_STACK_SIZE 16

#define C8_DISPLAY_WIDTH  	64
#define C8_DISPLAY_HEIGHT 	32
#define C8_DISPLAY_ROW    	8		// width / 8 bits
#define C8_DISPLAY_SIZE		256		// row * height
#define C8_FONT_HEIGHT		5

#define C8_ENTRY_POINT		0x200

typedef struct C8_ReservedArea {
	uint8_t v[0x10];		// 8-bit general purpose register
	uint8_t st, dt;			// timers
	uint16_t i;				// 16-bit index register
	
	uint16_t pc;			// program counter
	uint8_t sp;				// call stack pointer
	uint16_t stack[C8_STACK_SIZE];
	
	uint16_t keys;			// pressed keys
	
	uint8_t display[C8_DISPLAY_SIZE];
	uint8_t font[0x10 * C8_FONT_HEIGHT];
} c8_emu_reservedmem;

typedef union C8_Memory {
	uint8_t bytes[0x1000];
	c8_emu_reservedmem reserved;
} c8_emu_mem;

typedef struct C8_Emulator {
	c8_emu_mem memory;
} c8_emu;

/**
 * Initialise the virtual machine
 */
void C8_Emu_Init(c8_emu*);

/**
 * Execute the next instruction in the machine
 */
int C8_Emu_Cycle(c8_emu*);

/**
 * Tick down the timer (call this 60 times a second)
 */
void C8_Emu_Tick(c8_emu*);

#endif
