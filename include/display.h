#ifndef PACO_DSP_H
#define PACO_DSP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

struct Display {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

void DSP_draw(struct Display* display, bool screen[32][64]);
void DSP_init(struct Display* display);
void DSP_close(struct Display* display);

#endif
