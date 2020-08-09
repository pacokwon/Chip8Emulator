#ifndef PACO_EMU_H
#define PACO_EMU_H

#include "cpu.h"
#include "display.h"
#include "input.h"

struct Emulator {
    struct CPU* cpu;
    struct Display* display;
    struct Input* input;
};

void EMU_run(const char* filename);
void EMU_load(struct Emulator* emu, const char* filename);
void EMU_init(struct Emulator* emu);

#endif
