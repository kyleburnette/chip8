#include <SDL.h>

#include "chip8.h"

int main(int argc, char* args[]){
    struct Chip8 chip8;

    chip8_init(&chip8);
    chip8_load_game(&chip8, "ibm.ch8");
    chip8_run(&chip8);
    
    return 0;
}