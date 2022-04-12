#ifndef MEM_H
#define MEM_H

#include <stdint.h>

extern uint8_t* memory;
extern uint16_t stack[16];
extern uint8_t spr_addr[];

void init_memory(char*);

#endif