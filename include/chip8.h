#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TOTAL_MEMORY 4096
#define REGISTER_COUNT 16
#define START_ADDRESS 0x200
#define OPCODE_SIZE sizeof(uint16_t)
#define GFX_BUFFER_DIMENSIONS 64 * 32
#define PATH "./roms/"

struct Chip8 {
    uint16_t opcode;
    uint8_t memory[TOTAL_MEMORY];
    uint8_t V[REGISTER_COUNT];
    uint16_t indexRegister;
    uint16_t pc;
    uint32_t gfx[GFX_BUFFER_DIMENSIONS];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint16_t sp;
    uint8_t keys[16];
    bool draw_flag;
};

void chip8_init(struct Chip8* chip8);
bool chip8_load_game(struct Chip8* chip8, char* filename);
void chip8_handle_timers(struct Chip8* chip8);
void chip8_stack_push(struct Chip8* chip8);
void chip8_stack_pop(struct Chip8* chip8);
uint16_t chip8_fetch_opcode(struct Chip8* chip8);
void chip8_run_one_cycle(struct Chip8* chip8);

#endif // CHIP8_H