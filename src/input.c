#include "input.h"
#include <stdint.h>
#include <SDL2/SDL.h>

uint8_t KEYS[] = {
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_r,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_f,
    SDLK_z,
    SDLK_x,
    SDLK_c,
    SDLK_v
};

void INP_init(struct Input* input) {
    input->quit = false;
}

void INP_setKeys(struct Input* input, bool keys[16]) {
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT)
            input->quit = true;

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                input->quit = true;

            for (int i = 0; i < 16; i++) {
                if (event.key.keysym.sym == KEYS[i])
                    keys[i] = true;
            }
        }

        if (event.type == SDL_KEYUP) {
            for (int i = 0; i < 16; i++) {
                if (event.key.keysym.sym == KEYS[i])
                    keys[i] = false;
            }
        }
    }
}
