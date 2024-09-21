#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "chip8.h"
#include "mysdl.h"



int main(int argc, char** argv) {
	bool quit = false;
	srand((unsigned int)time(NULL));
	printf("Starting... \n\n");

	if (argc < 2) {

		printf("Usage: *Executable Name* <rom file> \n");
		return 0;

	}

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

		fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
		return 0;
	}

	window = SDL_CreateWindow(("CHIP8:  %s", argv[1]), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, 64, 32);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	initChip8();

	if (loadRom(argv[1]) == 0) {

		CleanUp_SDL(window, screen, renderer);
		return 0;
	}

	int speed = 5;
	printf("Press ESC to exit...\n");

	get_user_input(quit, speed, argv[1]);


	CleanUp_SDL(window, screen, renderer);

	return 0;
}