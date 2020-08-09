#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"

int main(int argc, char** argv) {
    if (argc >= 2) {
        struct Emulator* emu = malloc(sizeof(struct Emulator));
        EMU_run(emu, argv[argc - 1]);
        EMU_close(emu);
    } else {
        printf("ROM file not specified!");
        return 1;
    }

    return 0;
}
