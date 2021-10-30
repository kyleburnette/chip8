#include "window.h"
#include "constants.h"

void window_init(struct Window* window) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window->window = SDL_CreateWindow("Chip 8 Emulator", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * SCALE, 
        SCREEN_HEIGHT * SCALE, 
        SDL_WINDOW_SHOWN
    );

    window->renderer = SDL_CreateRenderer(
        window->window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    window->texture = SDL_CreateTexture(
        window->renderer,
        SDL_PIXELFORMAT_ARGB32,
        SDL_TEXTUREACCESS_STATIC,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    if (window->window == NULL || window->renderer == NULL || window->texture == NULL){
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
}

void window_draw(struct Window* window, struct Chip8* chip8){
    SDL_UpdateTexture(window->texture, NULL, chip8->gfx, SCREEN_WIDTH * sizeof(uint32_t));
    SDL_RenderCopy(window->renderer, window->texture, NULL, NULL);
    SDL_RenderPresent(window->renderer);
    chip8->draw_flag = false;
}