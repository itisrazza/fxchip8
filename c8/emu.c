#include "emu.h"

#include <stdlib.h>
#include <string.h>

#define V (emu->memory.reserved.v)
#define I (emu->memory.reserved.i)
#define ST (emu->memory.reserved.st)
#define DT (emu->memory.reserved.dt)
#define PC (emu->memory.reserved.pc)
#define SP (emu->memory.reserved.sp)
#define STACK (emu->memory.reserved.stack)
#define DISPLAY (emu->memory.reserved.display)
#define FONT (emu->memory.reserved.font)
#define KEY (emu->memory.reserved.key)

#define PEEK(addr) (emu->memory.bytes[addr])
#define POKE(addr, val) (emu->memory.bytes[addr] = (val))

// TODO: fill out
static uint8_t fontData[0x10 * C8_FONT_HEIGHT] = {
	0};

void C8_Emu_DisplayClear(c8_emu* emu);
uint8_t C8_Emu_Draw(c8_emu* emu, uint8_t x, uint8_t y, uint8_t length);

/**
 * Initialise the virtual machine
 */
void C8_Emu_Init(c8_emu *emu)
{
	int i;

	// clear out the registers
	for (i = 0; i < 0x10; i++)
		V[i] = 0x00;
	I = 0x0000;
	ST = 0x00;
	DT = 0x00;
	for (i = 0; i < C8_STACK_SIZE; i++)
		STACK[i] = 0x0000;
	KEY = 0xFF;

	// clear out the screen
	C8_Emu_DisplayClear(emu);

	// set the program counter to starting address
	PC = C8_ENTRY_POINT;

	// copy in the font data
	memcpy(FONT, fontData, sizeof(fontData));
}

/**
 * Execute the next instruction in the machine
 */
int C8_Emu_Cycle(c8_emu *emu)
{
	unsigned short temp;
	unsigned int i;

	unsigned char up = PEEK(PC);
	unsigned char lo = PEEK(PC + 1);
	#define nnn	((up & 0xF) | lo)
	#define u	((up >> 4) & 0xF)
	#define x	(up & 0xF)
	#define y	((lo >> 4) & 0xF)
	#define n   (lo & 0xF)
	#define kk	(lo)

	PC += 2;

	switch (u) {
	case 0:
		switch (nnn) {
		case 0x0E0:
			// CLS
			C8_Emu_DisplayClear(emu);
			break;

		case 0x0EE:
			// RET
			PC = STACK[SP];
			if (SP == 0) { 
				SP = C8_STACK_SIZE - 1;
			} else {
				SP--;
			} 
			break;

		default: return 1;
		}
		break;
	
	case 1:
		// JP addr
		PC = nnn;
		break;
	
	case 2:
		// CALL addr
		SP = (SP + 1) % C8_STACK_SIZE;
		STACK[SP] = PC;
		PC = nnn;
		break;
	
	case 3:
		// SE Vx, byte
		if (V[x] == kk) {
			PC += 2;
		}
		break;
	
	case 4:
		// SNE Vx, byte
		if (V[x] != kk) {
			PC += 2;
		}
		break;
	
	case 5:
		// SE Vx, Vy
		if (V[x] == V[y]) {
			PC += 2;
		}
		break;
	
	case 6:
		// LD Vx, byte
		V[x] = kk;
		break;
	
	case 7:
		// ADD Vx, byte
		V[x] += kk;
		break;
	
	case 8:
		switch (n) {
		case 0:
			// LD Vx, Vy
			V[x] = V[y];
			break;

		case 1:
			// OR Vx, Vy
			V[x] |= V[y];
			break;

		case 2:
			// AND Vx, Vy
			V[x] &= V[y];
			break;

		case 3:
			// XOR Vx, Vy
			V[x] ^= V[y];
			break;

		case 4:
			// ADD Vx, Vy
			temp = V[x] + V[y];
			V[x] = temp;
			V[0xF] = temp > 0xFF;
			break;

		case 5:
			// SUB Vx, Vy
			V[0xF] = V[x] > V[y];
			V[x] -= V[y];
			break;

		case 6:
			// SHR Vx, Vy
			V[0xF] = V[x] & 1;
			V[x] >>= 1;
			break;

		case 7:
			// SUBN Vx, Vy
			V[0xF] = V[y] > V[x];
			V[x] = V[y] - V[x];
			break;

		case 0xE:
			// SHL Vx, Vy
			V[0xF] = V[x] >> 7;
			V[x] <<= 1;
			break;

		default: return 1;
		}
		break;
	
	case 9:
		// SNE Vx, Vy
		if (V[x] != V[y]) {
			PC += 2;
		}
		break;
	
	case 0xA:
		// LD I, addr
		I = nnn;
		break;
	
	case 0xB:
		// JP V0, addr
		PC = nnn + V[0];
		break;
	
	case 0xC:
		// RND Vx, byte
		V[x] = rand() & kk;
		break;
	
	case 0xD:
		// DRW Vx, Vy, nibble
		V[0xF] = C8_Emu_Draw(emu, V[x], V[y], n);
		break;
	
	case 0xE:
		switch (kk) {
		case 0x9E:
			// SKP Vx
			if (V[x] == KEY) {
				PC += 2;
			}
			break;

		case 0xA1:
			// SKNP Vx
			if (V[x] != KEY) {
				PC += 2;
			}
			break;

		default: return 1;
		}
		break;
	
	case 0xF:
		switch (kk) {
		case 0x07:
			// LD Vx, DT
			V[x] = DT;
			break;
			
		case 0x0A:
			// LD Vx, K
			if (KEY != 0xFF) {
				V[x] = KEY;
			} else {
				PC -= 2;
			}
			break;
			
		case 0x15:
			// LD DT, Vx
			DT = V[x];
			break;
			
		case 0x18:
			// LD ST, Vx
			ST = V[x];
			break;
			
		case 0x1E:
			// ADD I, Vx
			I += V[x];
			break;
			
		case 0x29:
			// LD F, Vx
			I = offsetof(c8_emu_reservedmem, font) + V[x] * C8_FONT_HEIGHT;
			break;
			
		case 0x33:
			// LD B, Vx
			POKE(I + 0, 0);
			POKE(I + 1, 0);
			POKE(I + 2, 0);
			break;
			
		case 0x55:
			// LD [I], Vx
			for (i = 0; i <= x; i++) {
				POKE(I + i, V[i]);
			}
			break;
			
		case 0x65:
			// LD Vx, [I]
			for (i = 0; i <= x; i++) {
				V[i] = PEEK(I + i);
			}
			break;

		default: return 1;
		}
		break;
	
	default: return 1;
	}

	#undef nnn
	#undef u
	#undef x
	#undef y
	#undef n
	#undef kk
}

/**
 * Tick down the timer (call this 60 times a second)
 */
void C8_Emu_Tick(c8_emu *emu)
{
	if (ST > 0)
		ST--;
	if (DT > 0)
		DT--;
}

///

static void C8_Emu_DisplayClear(c8_emu* emu) {
	int i;
	for (i = 0; i < C8_DISPLAY_SIZE; i++)
		DISPLAY[i] = 0x00;
}

static uint8_t C8_Emu_Draw(c8_emu* emu, uint8_t x, uint8_t y, uint8_t length) {
	uint8_t line;
	uint8_t bitoffset = x;
	uint16_t screen, sprite;
	int i;

	for (i = 0; i < length; i++) {
		sprite = PEEK(I + i) >> x % 8;
	}

}
