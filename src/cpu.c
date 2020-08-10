#include "cpu.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 64
#define HEIGHT 32

void CPU_init(struct CPU* cpu) {
    cpu->opcode = 0;
    cpu->I = 0;
    cpu->sp = 0;
    cpu->pc = 0x200;
    cpu->delayTimer = 0;
    cpu->soundTimer = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->drawFlag = false;
    cpu->playSound = false;
    memset(cpu->V, 0, sizeof(cpu->V));
    memset(cpu->stack, 0, sizeof(cpu->stack));
    memset(cpu->keys, false, sizeof(cpu->keys));
    memset(cpu->display, false, sizeof(cpu->display));
    memset(cpu->memory, 0, sizeof(cpu->memory));
    uint8_t fonts[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0xF0, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for (int i = 0; i < 80; i++) {
        (cpu->memory)[i] = fonts[i];
    }
    srand(time(NULL));
}

void CPU_loadROM(struct CPU* cpu, const unsigned char* buffer) {
    CPU_init(cpu);
    memcpy(cpu->memory + 0x200, buffer, 512);
}

void CPU_clearDisplay(struct CPU* cpu) {
    memset(cpu->display, false, sizeof(cpu->display));
}

void CPU_runOperation(struct CPU* cpu) {
    // opcode is 16 bits
    cpu->opcode = (cpu->memory)[cpu->pc] << 8 | (cpu->memory)[cpu->pc + 1];

    // match the last 4 bits
    switch (cpu->opcode >> 12) {
        case 0x0:
            switch (cpu->opcode) {
                // CLS
                case 0x00E0:
                    CPU_clearDisplay(cpu);
                    cpu->pc += 2;
                    cpu->drawFlag = true;
                    break;
                // RET
                case 0x00EE:
                    cpu->sp -= 1;
                    cpu->pc = (cpu->stack)[cpu->sp] + 2;
                    break;
            }
            break;
        // JP addr
        case 0x1:
            cpu->pc = (cpu->opcode) & 0x0FFF;
            break;
        // CALL addr
        case 0x2:
            (cpu->stack)[cpu->sp] = cpu->pc;
            cpu->sp += 1;
            cpu->pc = (cpu->opcode) & 0x0FFF;
            break;
        // SE Vx, byte
        case 0x3: {
            uint8_t x = (cpu->opcode & 0x0F00) >> 8;
            uint16_t kk = cpu->opcode & 0x00FF;
            cpu->pc += 2;
            if ((cpu->V)[x] == kk)
                cpu->pc += 2;
            break;
        }
        // SNE Vx, byte
        case 0x4: {
            uint8_t x = (cpu->opcode & 0x0F00) >> 8;
            uint16_t kk = cpu->opcode & 0x00FF;
            cpu->pc += 2;
            if ((cpu->V)[x] != kk)
                cpu->pc += 2;
            break;
        }
        // SE Vx, Vy
        case 0x5: {
            uint8_t x = (cpu->opcode & 0x0F00) >> 8;
            uint8_t y = (cpu->opcode & 0x00F0) >> 4;
            cpu->pc += 2;
            if ((cpu->V)[x] == (cpu->V)[y])
                cpu->pc += 2;
            break;
        }
        // LD Vx, byte
        case 0x6:
            (cpu->V)[(cpu->opcode & 0x0F00) >> 8] = cpu->opcode & 0x00FF;
            cpu->pc += 2;
            break;
        // ADD Vx, byte
        case 0x7:
            (cpu->V)[(cpu->opcode & 0x0F00) >> 8] += cpu->opcode & 0x00FF;
            cpu->pc += 2;
            break;
        case 0x8: {
            uint8_t x = (cpu->opcode & 0x0F00) >> 8;
            uint8_t y = (cpu->opcode & 0x00F0) >> 4;

            switch (cpu->opcode & 0x000F) {
                // LD Vx, Vy
                case 0x0:
                    (cpu->V)[x] = (cpu->V)[y];
                    break;
                // OR Vx, Vy
                case 0x1:
                    (cpu->V)[x] |= (cpu->V)[y];
                    break;
                // AND Vx, Vy
                case 0x2:
                    (cpu->V)[x] &= (cpu->V)[y];
                    break;
                // XOR Vx, Vy
                case 0x3:
                    (cpu->V)[x] ^= (cpu->V)[y];
                    break;
                // ADD Vx, Vy
                case 0x4:
                    if ((cpu->V)[x] + (cpu->V)[y] > 0xFF)
                        (cpu->V)[0xF] = 1;
                    else
                        (cpu->V)[0xF] = 0;
                    (cpu->V)[x] += (cpu->V)[y];
                    break;
                // SUB Vx, Vy
                case 0x5:
                    if ((cpu->V)[x] > (cpu->V)[y])
                        (cpu->V)[0xF] = 1;
                    else
                        (cpu->V)[0xF] = 0;

                    (cpu->V)[x] -= (cpu->V)[y];
                    break;
                // SHR Vx
                case 0x6:
                    (cpu->V)[0xF] = (cpu->V)[x] & 1;
                    (cpu->V)[x] >>= 1;
                    break;
                // SUBN Vx, Vy
                case 0x7:
                    if ((cpu->V)[x] < (cpu->V)[y])
                        (cpu->V)[0xF] = 1;
                    else
                        (cpu->V)[0xF] = 0;

                    (cpu->V)[x] = (cpu->V)[y] - (cpu->V)[x];
                    break;
                // SHL Vx
                case 0xE:
                    (cpu->V)[0xF] = ((cpu->V)[x] & 0x80) >> 7;
                    (cpu->V)[x] <<= 1;
                    break;
            }
            cpu->pc += 2;
            break;
        }
        // SNE Vx, Vy
        case 0x9:
            cpu->pc += 2;
            if ((cpu->V)[(cpu->opcode & 0x0F00) >> 8] != (cpu->V)[(cpu->opcode & 0x00F0) >> 4])
                cpu->pc += 2;
            break;
        // LD I, addr
        case 0xA:
            cpu->I = cpu->opcode & 0x0FFF;
            cpu->pc += 2;
            break;
        // JP V0, addr
        case 0xB:
            cpu->pc = (cpu->V)[0] + cpu->opcode & 0x0FFF;
            break;
        // RND Vx, byte
        case 0xC:
            (cpu->V)[(cpu->opcode & 0x0F00) >> 8] = (cpu->opcode & 0x00FF) & (rand() % 0xFF);
            cpu->pc += 2;
            break;
        // DRW Vx, Vy, nibble
        case 0xD: {
            uint8_t x = (cpu->opcode & 0x0F00) >> 8;
            uint8_t y = (cpu->opcode & 0x00F0) >> 4;
            uint8_t n = (cpu->opcode & 0x000F);

            (cpu->V)[0xF] = 0;
            for (int h = 0; h < n; h++) {
                uint8_t sprite = (cpu->memory)[cpu->I + h];

                for (int w = 0; w < 8; w++) {
                    uint8_t px = ((cpu->V)[x] + w) % WIDTH;
                    uint8_t py = ((cpu->V)[y] + h) % HEIGHT;

                    // if XOR'ing causes any pixels to be erased, VF is set to 1, otherwise 0
                    // erased: 1 -> 0. display has to be 1, and it has to become 0
                    // this happens only when both the display's pixel and sprite's pixel are 1
                    /*
                    display pixel | result
                     0       0    |  0
                     0       1    |  1
                     1       0    |  1
                     1       1    |  0
                    */

                    // if sprite's pixel is bright
                    if (sprite & (0x80 >> w)) {
                        // if display's pixel is also bright, then
                        if ((cpu->display)[py][px])
                            (cpu->V)[0xF] = 1;

                        // display's pixel only changes when sprite's pixel is bright
                        (cpu->display)[py][px] ^= 1;
                    }
                }
            }
            cpu->drawFlag = true;
            cpu->pc += 2;
            break;
        }
        case 0xE:
            switch (cpu->opcode & 0x00FF) {
                // SKP Vx
                case 0x9E:
                    cpu->pc += 2;
                    if ((cpu->keys)[(cpu->opcode & 0x0F00) >> 8])
                        cpu->pc += 2;
                    break;
                // SKNP Vx
                case 0xA1:
                    cpu->pc += 2;
                    if (!(cpu->keys)[(cpu->opcode & 0x0F00) >> 8])
                        cpu->pc += 2;
                    break;
            }
            break;
        case 0xF:
            switch (cpu->opcode & 0x00FF) {
                // LD Vx, DT
                case 0x07:
                    (cpu->V)[(cpu->opcode & 0x0F00) >> 8] = cpu->delayTimer;
                    cpu->pc += 2;
                    break;
                // LD Vx, K
                case 0x0A: {
                    bool pressed = false;

                    for (int i = 0; i < 16; i++)
                        if ((cpu->keys)[i]) {
                            (cpu->V)[(cpu->opcode & 0x0F00) >> 8] = i;
                            pressed = true;
                        }

                    if (pressed)
                        cpu->pc += 2;
                    else
                        return;
                    break;
                }
                // LD DT, Vx
                case 0x15:
                    cpu->delayTimer = (cpu->V)[(cpu->opcode & 0x0F00) >> 8];
                    cpu->pc += 2;
                    break;
                // LD ST, Vx
                case 0x18:
                    cpu->soundTimer = (cpu->V)[(cpu->opcode & 0x0F00) >> 8];
                    cpu->pc += 2;
                    break;
                // ADD I, Vx
                case 0x1E:
                    if (cpu->I + (cpu->V)[(cpu->opcode & 0x0F00) >> 8] > 0xFF)
                        (cpu->V)[0xF] = 1;
                    else
                        (cpu->V)[0xF] = 0;
                    cpu->I += (cpu->V)[(cpu->opcode & 0x0F00) >> 8];
                    cpu->pc += 2;
                    break;
                // LD F, Vx
                case 0x29:
                    cpu->I = (cpu->V)[(cpu->opcode & 0x0F00) >> 8] * 5;
                    cpu->pc += 2;
                    break;
                // LD B, Vx
                case 0x33: {
                    uint8_t vx = (cpu->V)[(cpu->opcode & 0x0F00) >> 8];
                    (cpu->memory)[cpu->I + 0] = vx / 100;
                    (cpu->memory)[cpu->I + 1] = (vx / 10) % 10;
                    (cpu->memory)[cpu->I + 2] = vx % 10;
                    cpu->pc += 2;
                    break;
                }
                // LD [I], Vx
                case 0x55: {
                    uint8_t x = (cpu->opcode & 0x0F00) >> 8;

                    for (int i = 0; i <= x; i++)
                        (cpu->V)[i] = (cpu->memory)[cpu->I + i];

                    cpu->I += x + 1;
                    cpu->pc += 2;
                    break;
                }
                // LD Vx, [I]
                case 0x65: {
                    uint8_t x = (cpu->opcode & 0x0F00) >> 8;

                    for (int i = 0; i <= x; i++)
                        (cpu->memory)[cpu->I + i] = (cpu->V)[i];

                    cpu->I += x + 1;
                    cpu->pc += 2;
                    break;
                }
            }
            break;
    }

    if (cpu->delayTimer > 0)
        cpu->delayTimer -= 1;

    if (cpu->soundTimer > 0) {
        cpu->soundTimer -= 1;
        if (cpu->soundTimer == 0)
            cpu->playSound = true;
    }
}
