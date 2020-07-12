#ifndef PACO_EMU_H
#define PACO_EMU_H

struct Emulator {
    struct CPU* cpu;
};

void EMU_run(struct Emulator* emu, const char* filename);
void EMU_load(struct Emulator* emu, const char* filename);
void EMU_init(struct Emulator* emu);

#endif
