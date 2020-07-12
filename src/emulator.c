#include "emulator.h"
#include "cpu.h"
#include <stdio.h>

#define MEMORY_SIZE 4096

void EMU_run(struct Emulator* emu, const char* filename) {

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

void EMU_init(struct Emulator* emu) {}
