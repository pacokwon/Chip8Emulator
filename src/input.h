#ifndef PACO_INP_H
#define PACO_INP_H

#include <stdbool.h>

struct Input {
    bool quit;
};

void INP_init(struct Input* input);
void INP_setKeys(struct Input* input, bool keys[16]);

#endif
