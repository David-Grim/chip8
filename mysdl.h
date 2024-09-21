#ifndef MYSDL
#define MYSDL

#include <SDL.h>
#include <stdbool.h>


SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* screen;
SDL_Event event;
void Draw(SDL_Window* window, SDL_Texture* screen, SDL_Renderer* renderer);
void CleanUp_SDL(SDL_Window* window, SDL_Texture* screen, SDL_Renderer* renderer);
int loadRom(char* file);
void get_user_input(bool exit_flag, int speed, char* rom);

#endif