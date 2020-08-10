#include "display.h"

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 640;

void DSP_draw(struct Display* display, bool screen[32][64]) {
    int wUnit = SCREEN_WIDTH / 64;
    int hUnit = SCREEN_HEIGHT / 32;

    SDL_SetRenderDrawColor(display->renderer, 0x00, 0x00, 0x00, 0xFF); // black
    SDL_RenderClear(display->renderer);
    SDL_SetRenderDrawColor(display->renderer, 0xFF, 0xFF, 0xFF, 0xFF); // white

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            if (screen[i][j]) {
                SDL_Rect rect = { j * wUnit, i * hUnit, wUnit, hUnit };
                SDL_RenderFillRect(display->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(display->renderer);
}

void DSP_init(struct Display* display) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        printf("%s", SDL_GetError());
        return;
    }

    display->window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (display->window == NULL) {
        printf("%s", SDL_GetError());
        return;
    }

    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (display->renderer == NULL) {
        printf("%s", SDL_GetError());
        return;
    }
}

void DSP_close(struct Display* display) {
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}
