#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include "mysdl.h"
#include "chip8.h"



// Load rom file into memory
int loadRom(char* file) {
	FILE* fp = fopen(file, "rb");

	if (fp == NULL) {
		fprintf(stderr, "Can't open the file rom \n");
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(memory + 0x200, sizeof(int), size, fp);

	return 1;
}

// Draw function
void Draw(SDL_Window* window, SDL_Texture* screen, SDL_Renderer* renderer) {

	uint32_t pixels[64 * 32];

	if (drawflag) {

		memset(pixels, 0, 8192);

		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 64; x++) {
				int index = x + (y * 64);
				if (gfx[index] == 1) {
					pixels[index] = UINT32_MAX; // Set to maximum value for white color
				}
			}
		}

		SDL_UpdateTexture(screen, NULL, pixels, 64 * sizeof(uint32_t));

		SDL_Rect position = {0,0,0,0};
		position.x = 0;
		position.y = 0;

		position.w = 64;
		position.h = 32;
		SDL_RenderCopy(renderer, screen, NULL, &position);
		SDL_RenderPresent(renderer);
	}
	drawflag = 0;
}


void CleanUp_SDL(SDL_Window* window, SDL_Texture* screen, SDL_Renderer* renderer) {
	SDL_DestroyTexture(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void get_user_input(bool exit_flag, int speed, char* rom) {
	while (!exit_flag) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

				case SDL_QUIT:
					exit_flag = true;
				break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {

					//exit
						case SDLK_ESCAPE:
							exit_flag = true;
						break;
					//restart
						case SDLK_F1:
							initChip8();
							if (loadRom(rom) == 0) {
								CleanUp_SDL(window, screen, renderer);
							}
						break;

					//decrease speed
						case SDLK_UP:
							speed -= 1;
							break;
					//increase speed
						case SDLK_DOWN:
							speed += 1;
							break;
					//controls
						case SDLK_x:
							keyboard[0] = 1;
							break;
						case SDLK_1:
							keyboard[1] = 1;
							break;
						case SDLK_2:
							keyboard[2] = 1;
							break;
						case SDLK_3:
							keyboard[3] = 1;
							break;
						case SDLK_q:
							keyboard[4] = 1;
							break;
						case SDLK_w:
							keyboard[5] = 1;
							break;
						case SDLK_e:
							keyboard[6] = 1;
							break;
						case SDLK_a:
							keyboard[7] = 1;
							break;
						case SDLK_s:
							keyboard[8] = 1;
							break;
						case SDLK_d:
							keyboard[9] = 1;
							break;
						case SDLK_z:
							keyboard[0xA] = 1;
							break;
						case SDLK_c:
							keyboard[0xB] = 1;
							break;
						case SDLK_4:
							keyboard[0xC] = 1;
							break;
						case SDLK_r:
							keyboard[0xD] = 1;
							break;
						case SDLK_f:
							keyboard[0xE] = 1;
							break;
						case SDLK_v:
							keyboard[0xF] = 1;
							break;
					}
					break;

					case SDL_KEYUP:
						switch (event.key.keysym.sym) {
						case SDLK_x:
							keyboard[0] = 0;
							break;
						case SDLK_1:
							keyboard[1] = 0;
							break;
						case SDLK_2:
							keyboard[2] = 0;
							break;
						case SDLK_3:
							keyboard[3] = 0;
							break;
						case SDLK_q:
							keyboard[4] = 0;
							break;
						case SDLK_w:
							keyboard[5] = 0;
							break;
						case SDLK_e:
							keyboard[6] = 0;
							break;
						case SDLK_a:
							keyboard[7] = 0;
							break;
						case SDLK_s:
							keyboard[8] = 0;
							break;
						case SDLK_d:
							keyboard[9] = 0;
							break;
						case SDLK_z:
							keyboard[0xA] = 0;
							break;
						case SDLK_c:
							keyboard[0xB] = 0;
							break;
						case SDLK_4:
							keyboard[0xC] = 0;
							break;
						case SDLK_r:
							keyboard[0xD] = 0;
							break;
						case SDLK_f:
							keyboard[0xE] = 0;
							break;
						case SDLK_v:
							keyboard[0xF] = 0;
							break;
						}
						break;
			}
			break;
		}

		SDL_Delay(speed);	

		if (delay_timer > 0) {
			delay_timer--;
		}
		Draw(window, screen, renderer);
		Emulate();
	}
}