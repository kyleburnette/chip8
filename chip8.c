#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TOTAL_MEMORY 4096
#define REGISTER_COUNT 16
#define START_ADDRESS 0x200
#define OPCODE_SIZE sizeof(uint16_t)
#define GFX_BUFFER_DIMENSIONS 64 * 32

struct Chip8 {
    uint16_t opcode;
    uint8_t memory[TOTAL_MEMORY];
    uint8_t V[REGISTER_COUNT];
    uint16_t indexRegister;
    uint16_t pc;
    uint8_t gfx[GFX_BUFFER_DIMENSIONS];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint16_t sp;
    uint8_t key[16];
};

bool chip8_load_game(struct Chip8* chip8, char* filename){
    FILE* file = fopen(filename, "r");

    fread(chip8->memory + START_ADDRESS, TOTAL_MEMORY - START_ADDRESS, 1, file);
    fclose(file);
}

void chip8_init(struct Chip8* chip8){
    chip8->pc = START_ADDRESS;
}

uint16_t chip8_fetch_opcode(struct Chip8* chip8){
    uint16_t opcode;

    memcpy(&opcode, &chip8->memory[chip8->pc], OPCODE_SIZE);
    opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;
    return opcode;
}

void chip8_run(struct Chip8* chip8){
    while (chip8->pc < 0x210){
        chip8->opcode = chip8_fetch_opcode(chip8);
        switch(chip8->opcode & 0xF000){
            case 0x0000:
                switch (chip8->opcode & 0x000F){
                    case 0x0000:
                        memset(chip8->gfx, 0, GFX_BUFFER_DIMENSIONS);
                        break;
                    case 0x000E:
                        break;
                }
                break;
            case 0x1000:
                chip8->pc = chip8->opcode & 0x0FFF;
                break;
            case 0x2000:
                printf("0x2000\n");
                break;
            case 0x3000:
                printf("0x3000\n");
                break;
            case 0x4000: {
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;
                int8_t valueInReg = chip8->V[reg];
                int8_t testValue = chip8->opcode & 0x00FF;

                if (valueInReg != testValue){
                    chip8->pc += 2; // skip next instruction
                }
                
                break;
            }
                
            case 0x5000:
                printf("0x5000\n");
                break;
            case 0x6000: {
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;
                int8_t value = chip8->opcode & 0x00FF;

                chip8->V[reg] = value;
                break;
            }

            case 0x7000:
                printf("0x7000\n");
                break;
            case 0x8000:
                printf("0x8000\n");
                break;
            case 0x9000:
                printf("0x9000\n");
                break;
            case 0xA000: {
                int8_t value = chip8->opcode & 0x0FFF;

                chip8->indexRegister = value;
                break;
            }

            case 0xB000:    
                printf("0xB000\n");
                break;
            case 0xC000:
                printf("0xC000\n");
                break;
            case 0xD000:
                printf("0xD000\n");
                break;
            case 0xE000:
                printf("0xE000\n");
                break;
            case 0xF000:
                printf("0xF000\n");
                break;
            default:
                break;
        }

    }
}

int main(){
    struct Chip8 chip8;

    chip8_init(&chip8);
    chip8_load_game(&chip8, "15puzzle.ch8");
    chip8_run(&chip8);
    
    return 0;
}