#ifndef PACO_DSP_H
#define PACO_DSP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

int WIDTH = 1280;
int HEIGHT = 640;

struct Display {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

void DSP_draw(struct Display* display, bool screen[32][64]);
void DSP_init(struct Display* display);
void DSP_close(struct Display* display);

#endif
