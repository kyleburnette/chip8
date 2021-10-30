#include "chip8.h"
#include "window.h"

int main(int argc, char* argv[]){
    struct Chip8 chip8;
    struct Window window;
    bool running = true;

    chip8_init(&chip8);
    window_init(&window);
    chip8_load_game(&chip8, "ibm.ch8");

    while(running){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if( event.type == SDL_QUIT ) running = false;
        }
        chip8_run_one_cycle(&chip8);
        if(chip8.draw_flag){
            window_draw(&window, &chip8);
        }
    }
    
    return 0;
}