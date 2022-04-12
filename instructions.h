#ifndef INST_H
#define INST_H

#include <stdint.h>

uint16_t stack_top();
uint16_t stack_pop();
void stack_push(uint16_t);

void inst_00E0(void);
void inst_00EE(void);
void inst_0nnn(uint16_t);
void inst_1nnn(uint16_t);
void inst_2nnn(uint16_t);
void inst_3xkk(uint8_t*,uint8_t);
void inst_4xkk(uint8_t*,uint8_t);
void inst_5xy0(uint8_t*,uint8_t*);
void inst_6xkk(uint8_t*,uint8_t);
void inst_7xkk(uint8_t*,uint8_t);
void inst_8xy0(uint8_t*,uint8_t*);
void inst_8xy1(uint8_t*,uint8_t*);
void inst_8xy2(uint8_t*,uint8_t*);
void inst_8xy3(uint8_t*,uint8_t*);
void inst_8xy4(uint8_t*,uint8_t*);
void inst_8xy5(uint8_t*,uint8_t*);
void inst_8xy6(uint8_t*,uint8_t*);
void inst_8xy7(uint8_t*,uint8_t*);
void inst_8xyE(uint8_t*,uint8_t*);
void inst_9xy0(uint8_t*,uint8_t*);
void inst_Annn(uint16_t);
void inst_Bnnn(uint16_t);
void inst_Cxkk(uint8_t*,uint8_t);
void inst_Dxyn(uint8_t*,uint8_t*,uint8_t);
void inst_Ex9E(uint8_t*);
void inst_ExA1(uint8_t*);
void inst_Fx07(uint8_t*);
void inst_Fx0A(uint8_t*);
void inst_Fx15(uint8_t*);
void inst_Fx18(uint8_t*);
void inst_Fx1E(uint8_t*);
void inst_Fx29(uint8_t*);
void inst_Fx33(uint8_t*);
void inst_Fx55(uint8_t);
void inst_Fx65(uint8_t);

#endif