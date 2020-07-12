#ifndef PACO_CPU_H
#define PACO_CPU_H

#include <stdint.h>
#include <stdbool.h>

struct CPU {
    uint16_t opcode;
    uint8_t V[16];
    uint16_t stack[16];
    uint16_t I;
    uint8_t sp;
    uint16_t pc;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t X;
    uint8_t Y;
    uint8_t memory[4096];
    uint8_t fonts[80];
    bool keys[16];
    bool display[32][64];
    bool drawFlag;
};

void CPU_runOperation(struct CPU* cpu);
void CPU_initialize(struct CPU* cpu);
void CPU_clearDisplay(struct CPU* cpu);
void CPU_loadROM(struct CPU* cpu, const unsigned char buffer[512]);

#endif
