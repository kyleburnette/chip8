#ifndef WINDOW_H
#define WINDOW_H

#define SDL_MAIN_HANDLED
#include <SDL2\SDL.h>
#include "chip8.h"

struct Window {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
};

void window_init(struct Window* window);
void window_draw(struct Window* window, struct Chip8* chip8);

#endif // WINDOW_H