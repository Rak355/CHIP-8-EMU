#ifndef CPU_H
#define CPU_H

#include <stdint.h>

extern struct Cpu{
    uint16_t PC;
    uint8_t SP;
    uint8_t V[16];
    uint16_t I;
    uint8_t DT, ST;
}cpu;

void cpu_cycle(char*);
void timer_cycle();
void init_cpu();

#endif