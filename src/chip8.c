#include "chip8.h"
#include "error.h"

void chip8_init(struct Chip8* chip8){
    chip8->pc = START_ADDRESS;
    chip8->sp = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->draw_flag = false;

    printf("Chip 8 initialized.\n");
}

bool chip8_load_game(struct Chip8* chip8, char* rom_name){
    char length = strlen(PATH) + strlen(rom_name) + 1;
    char* filename = (char*)malloc(length * sizeof(char));
    sprintf(filename, "%s%s", PATH, rom_name);
    FILE* file = fopen(filename, "r");

    if(!file){
        perror("Could not open ROM!\n");
        exit(ROM_OPEN_ERROR);
    } else {
        printf("Opened ROM: %s\n", filename);
        free(filename);
    }

    fread(chip8->memory + START_ADDRESS, TOTAL_MEMORY - START_ADDRESS, 1, file);
    fclose(file); 
}

void chip8_handle_timers(struct Chip8* chip8){
    if (chip8->delay_timer > 0){
        chip8->delay_timer--;
    }
    if (chip8->sound_timer > 0){
        chip8->sound_timer--;
    }
}

void chip8_stack_push(struct Chip8* chip8){
    chip8->stack[chip8->sp] = chip8->pc; // store current pc
    chip8->sp++; // increment stack pointer
}

void chip8_stack_pop(struct Chip8* chip8){
    chip8->sp--;
}

uint16_t chip8_fetch_opcode(struct Chip8* chip8){
    uint16_t opcode;

    memcpy(&opcode, &chip8->memory[chip8->pc], OPCODE_SIZE);
    opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    chip8->pc += 2;
    return opcode;
}

void chip8_run_one_cycle(struct Chip8* chip8){
        chip8->opcode = chip8_fetch_opcode(chip8);
        switch(chip8->opcode & 0xF000){
            case 0x0000:
                switch (chip8->opcode & 0x000F){
                    case 0x0000:
                        printf("Clearing graphics buffer...\n");
                        memset(chip8->gfx, 1, GFX_BUFFER_DIMENSIONS * sizeof(chip8->gfx[0]));
                        break;
                    case 0x000E: {
                        chip8->pc = chip8->stack[chip8->sp];
                        chip8_stack_pop(chip8);
                        break;
                    }
                        
                }
                break;
            case 0x1000:
                chip8->pc = chip8->opcode & 0x0FFF;
                break;
            case 0x2000: { // call subroutine
                uint16_t srMMLoc = chip8->opcode & 0x0FFF;

                chip8_stack_push(chip8);
                chip8->pc = srMMLoc; // jump to subroutine
                break;
            }

            case 0x3000: { 
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;
                uint8_t valueInReg = chip8->V[reg];
                uint8_t testValue = chip8->opcode & 0x00FF;

                if (valueInReg == testValue){
                    chip8->pc += 2; // skip next instruction
                }
                
                break;
            }

            case 0x4000: {
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;
                uint8_t valueInReg = chip8->V[reg];
                uint8_t testValue = chip8->opcode & 0x00FF;

                if (valueInReg != testValue){
                    chip8->pc += 2; // skip next instruction
                }
                
                break;
            }
                
            case 0x5000: {
                uint8_t regX = (chip8->opcode & 0x0F00) >> 8;
                uint8_t regY = (chip8->opcode & 0x00F0) >> 4;

                if (chip8->V[regX] == chip8->V[regY]){
                    chip8->pc += 2; // skip next instruction
                }
                break;
            }
                
            case 0x6000: { // set register
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;
                uint8_t value = chip8->opcode & 0x00FF;

                chip8->V[reg] = value;
                break;
            }

            case 0x7000: { // add value to register
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;
                uint8_t value = chip8->opcode & 0x00FF;
                
                chip8->V[reg] += value;
                break;
            }
                
            case 0x8000: {
                uint8_t regX = (chip8->opcode & 0x0F00) >> 8;
                uint8_t regY = (chip8->opcode & 0x00F0) >> 4;

                switch (chip8->opcode & 0x000F){
                    

                    case 0x0: {
                        chip8->V[regX] = chip8->V[regY];
                        break;
                    }
                    case 0x1: {
                        chip8->V[regX] |= chip8->V[regY];
                        break;
                    }
                    case 0x2: {
                        chip8->V[regX] &= chip8->V[regY];
                        break;
                    }
                    case 0x3: {
                        chip8->V[regX] ^= chip8->V[regY];
                        break;
                    }
                    case 0x4: {
                        uint16_t total = chip8->V[regX] + chip8->V[regY];

                        chip8->V[0xF] = (total > 255);
                        chip8->V[regX] += chip8->V[regY];
                        break;
                    }
                    case 0x5: {
                        break;
                    }
                    case 0x6: {
                        break;
                    }
                    case 0x7: {
                        break;
                    }
                    case 0xE: {
                        break;
                    }
                }

                break;
                }
                
            case 0x9000: {
                uint8_t regX = (chip8->opcode & 0x0F00) >> 8;
                uint8_t regY = (chip8->opcode & 0x00F0) >> 4;

                if (chip8->V[regX] != chip8->V[regY]){
                    chip8->pc += 2;
                }

                break;
            }

            case 0xA000: { // set index register
                uint16_t value = chip8->opcode & 0x0FFF;

                chip8->indexRegister = value;
                break;
            }

            case 0xB000: {
                uint16_t value = chip8->opcode & 0x0FFF;

                chip8->pc = chip8->V[0] + value;
                break;
            }

            case 0xC000: {
                uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                chip8->V[reg] = rand() & chip8->opcode & 0x00FF;
                break;
            }

            case 0xD000: { // display
                uint8_t regX = (chip8->opcode & 0x0F00) >> 8;
                uint8_t regY = (chip8->opcode & 0x00F0) >> 4;
                uint8_t height = (chip8->opcode & 0x000F);
                uint8_t width = 8;
                uint8_t coordX = chip8->V[regX];
                uint8_t coordY = chip8->V[regY];
                chip8->V[0xF] = 0;

                for (int i = 0; i < height; i++){
                    for (int j = 0; j < 8; j++){
                        uint8_t pixel = chip8->memory[chip8->indexRegister + i];
                        if (pixel & (0x80 >> j)){
                            int index = (chip8->V[regX] + j) % 64 + ((chip8->V[regY] + i) % 32) * 64;
                            if (chip8->gfx[index] == 1){
                                chip8->V[0xF] = 1;
                                chip8->gfx[index] = 0xFF000000;
                            } else {
                                chip8->gfx[index] = 0xFFFFFFFF;
                            }
                            chip8->draw_flag = true;
                        }
                    }
                }
                break;
            }
                
            case 0xE000:
                printf("0xE000\n");
                break;
            case 0xF000: {
                switch (chip8->opcode & 0x00FF){
                    case 0x07: {
                        uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                        chip8->V[reg] = chip8->delay_timer;
                        break;
                    }

                    case 0x0A:
                        break;
                    case 0x15: {
                        uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                        chip8->delay_timer = chip8->V[reg];
                        break;
                    }
                        
                    case 0x18: {
                        uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                        chip8->sound_timer = chip8->V[reg];
                        break;
                    }

                    case 0x1E: {
                        uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                        chip8->indexRegister += chip8->V[reg];
                        break;
                    }
                        
                    case 0x29:
                        break;
                    case 0x33:
                        break;
                    case 0x55: {
                        uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                        for (int i = 0; i < reg; i++){
                            chip8->memory[chip8->indexRegister + i] = chip8->V[i];
                        }
                        break;
                    }
                        
                    case 0x65: {
                        uint8_t reg = (chip8->opcode & 0x0F00) >> 8;

                        for (int i = 0; i < reg; i++){
                            chip8->V[i] = chip8->memory[chip8->indexRegister + i];
                        }
                        break;
                    }
                }
            }
                printf("0xF000\n");
                break;
            default:
                break;
        
    chip8_handle_timers(chip8);
    }
}