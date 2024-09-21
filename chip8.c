#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"


//Initialize everything
 void initChip8(void) {
	opcode = 0;
	memset(memory, 0, 4096);
	memset(v, 0, 16);
	I = 0;
	PC = 0x200;
	memset(gfx, 0, 2048);
	delay_timer = 0;
	sound_timer = 0;
	memset(stack, 0, 16);
	sp = 0;
	drawflag = false;
	memset(keyboard, 0, 16);
	memcpy(memory, chip8_fontset, 80 * sizeof(int8_t));
}




// Emulate cycle
void Emulate(void){
	uint8_t X, Y, n;
	uint16_t nnn;

	opcode = memory[PC] << 8 | memory[PC + 1];
	PC += 2;
	X = (opcode & 0x0F00) >> 8;
	Y = (opcode & 0x00F0) >> 4;
	nnn = (opcode & 0x0FFF);
	n = (opcode & 0x000F);

	switch (opcode & 0xF000){
		case 0x0000:
			switch (opcode & 0x00FF) {
				// 00E0 
				case 0x00E0:
					memset(gfx, 0, 2048);
					drawflag = true;
					break;

				//00E0 
				case 0x00EE:
					sp--;
					PC = stack[sp];
					break;

				default: 
					printf("Opcode error 0xxx -> 0x%x\n", opcode);

			}
		break;

		//1nnn 
		case 0x1000:
			PC = nnn;
		break;

		//2nnn 
		case 0x2000:
			stack[sp] = PC;
			sp++;
			PC = nnn;
		break;

			//3xnn
		case 0x3000:
			if (v[X] == (opcode & 0x00FF)) {
				PC += 2;
			}
		break;

		//4xnn
		case 0x4000:
			if (v[X] != (opcode & 0x00FF)) {
				PC += 2;
			}
		break;

		//5xy0
		case 0x5000:
			if (v[X] == v[Y]) {
				PC += 2;
			}
		break;

		//6xnn
		case 0x6000:
			v[X] = (opcode & 0x00FF);
		break;

		//7xnn
		case 0x7000:
			v[(X)] += (opcode & 0x00FF);
		break;

		//8xyn
		case 0x8000:
			switch (n) {

				//8xy0
				case 0x0000:
					v[X] = v[Y];
				break;

				//8xy1
				case 0x0001:
					v[X] |= v[Y];
				break;

				//8xy2
				case 0x0002:
					v[X] &= v[Y];
				break;

				//8xy3
				case 0x0003:
					v[X] ^= v[Y];
				break;

				//8xy4
				case 0x0004: {
					int i = (int)(v[X]) + (int)(v[Y]);

					if (i > 255) {
						v[0xF] = 1;
					}
					else {
						v[0xF] = 0;
					}
					v[X] = i & 0xFF;
				}
				break;

				//8xy5
				case 0x0005:
					if (v[X] > v[Y]) {
						v[0xF] = 1;
					}
					else {
						v[0xF] = 0;
					}
					v[X] -= v[Y];
				break;

				//8xy6
				case 0x0006:
					v[0xF] = v[X] & 1;
					v[X] >>= 1;
				break;

				//8xy7
				case 0x0007:
					if (v[Y] > v[X]) {
						v[0xF] = 1;
					}
					else {
						v[0xF] = 0;
					}
					v[X] = v[Y] - v[X];
				break;

				//8xyE
				case 0x000E:
					v[0xF] = v[X] >> 7;
					v[X] <<= 1;
				break;

				default: 
					printf("ERROR OCCURED AT OPCODE 8xxx : %x\n", opcode);

			}
			break;

			//9xy0
			case 0x9000:
				if (v[X] != v[Y]) {
					PC += 2;
				}
			break;

			//Annn
			case 0xA000:
				I = nnn;
			break;

			//Bnnn
			case 0xB000:
				PC = (nnn) + v[X];
			break;

			//Cxnn
			case 0xC000: {
				v[X] = (rand() % 0x100) & (opcode & 0x00FF);
			}
			break;

			//Dxyn
			case 0xD000: {

				v[0xF] = 0;
				int height = n;

				int baseIndex = (v[Y] * 64) + v[X];

				for (int yline = 0; yline < height; yline++) {
					int pixel = memory[I + yline];
					int rowIndex = baseIndex + (yline * 64);
					for (int xline = 0; xline < 8; xline++) {
						if ((pixel & (0x80 >> xline)) != 0) {
							int currentIndex = rowIndex + xline;
							if (gfx[currentIndex] == 1) {
								v[0xF] = 1;
							}
							gfx[currentIndex] ^= 1;
						}
					}
				}
				drawflag = true;
			}
			break;

	//Exnn
	case 0xE000:
		switch ((opcode & 0x00FF)) {
			//Ex9E
			case 0x009E:
				if (keyboard[v[X]] != 0) {
					PC += 2;
				}
				break;

			//ExA1
			case 0x00A1:
				if (keyboard[v[X]] == 0) {
					PC += 2;
				}
				break;

		}
		break;

	//Fxnn
	case 0xF000:
		switch ((opcode & 0x00FF)) {

			//Fx07
			case 0x0007:
				v[X] = delay_timer;
			break;

			//Fx0A
			case 0x000A: {
				int key_is_pressed = 0;
				for (int i = 0; i < 16; i++) {
					if (keyboard[i]) {
						key_is_pressed = 1;
						v[X] = i;
						break;
					}
				}

				if (!key_is_pressed) {
					PC -= 2;
				}
			}
			break;

			//Fx15
			case 0x0015:
				delay_timer = v[X];
			break;

			//Fx18
			case 0x0018:
				sound_timer = v[X];
			break;

			//Fx1E
			case 0x001E:
				I = I + v[X];
			break;

			//Fx29
			case 0x0029:
				I = v[X] * 5;
			break;

			//Fx33
			case 0x0033: {
				memory[I] = v[X] / 100;       
				v[X] %= 100;
				memory[I + 1] = v[X] / 10;
				memory[I + 2] = v[X] % 10;
			}
			break;

			//Fx55
			case 0x0055:
				for (int i = 0; i <= X; i++) {
					memory[I + i] = v[i];
				}
			break;

			//Fx65
			case 0x0065:
				for (int i = 0; i <= X; i++) {
					v[i] = memory[I + i];
				}
			break;
		}
		break;

	default: 
		printf("ERROR OCCURED AT OPCODE: %x \n", opcode); 
		break;
	}
}
