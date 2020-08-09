#include "emulator.h"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define MEMORY_SIZE 4096

void EMU_run(const char* filename) {
    struct Emulator* emu = malloc(sizeof(struct Emulator));
    EMU_init(emu);
    EMU_load(emu, filename);

    while (true) {
        CPU_runOperation(emu->cpu);

        if (emu->cpu->drawFlag) {
            DSP_draw(emu->display, emu->cpu->display);
            emu->cpu->drawFlag = false;
        }

        if (emu->cpu->playSound) {
            printf("\a");
            emu->cpu->playSound = false;
        }

        INP_setKeys(emu->input, emu->cpu->keys);

        usleep(1200 * 1000);
    }
}

void EMU_load(struct Emulator* emu, const char* filename) {
    FILE* file = fopen(filename, "rb");
    unsigned char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    if (size > MEMORY_SIZE - 0x200) {
        // TODO: throw error
    }

    fread(buffer, size, 1, file);
    fclose(file);
    CPU_loadROM(emu->cpu, (const unsigned char*)buffer);
}

void EMU_init(struct Emulator* emu) {
    emu->cpu = malloc(sizeof(struct CPU));
    emu->display = malloc(sizeof(struct Display));
    emu->input = malloc(sizeof(struct Input));
    CPU_init(emu->cpu);
    DSP_init(emu->display);
    INP_init(emu->input);
}
