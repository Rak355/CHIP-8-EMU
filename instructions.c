#include "instructions.h"
#include "memory.h"
#include "cpu.h"
#include "sdl_main.h"
#include "display.h"

#include <time.h>
#include <stdlib.h>

typedef unsigned int uint; 

// stack top
uint16_t stack_top(){
    return stack[cpu.SP];
}

// stack pop
uint16_t stack_pop(){
    uint16_t ret_val = stack[cpu.SP--];
    return ret_val;
}

// stack push
void stack_push(uint16_t value){
    stack[++cpu.SP] = value;
}

// Jump to a machine code routine at nnn
void inst_0nnn(uint16_t nnn){
    cpu.PC = nnn;
}

// clear the display
void inst_00E0(void){
    clear_screen();
}

// return from a subroutine
void inst_00EE(void){
    /*
        The interpreter sets the program counter 
        to the address at the top of the stack, 
        then subtracts 1 from the stack pointer.
    */
    cpu.PC = stack_pop();
}

// jump to location nnn
void inst_1nnn(uint16_t nnn){
    // Interpreter sets the program counter to nnn.
    cpu.PC = nnn;
}

// call subroutine at nnn
void inst_2nnn(uint16_t nnn){
    /*
        The interpreter increments the stack pointer,
        then puts the current PC on the top of the stack. 
        The PC is then set to nnn.
    */
    stack_push(cpu.PC);
    cpu.PC = nnn;
}

// Skip next instruction if Vx = kk
void inst_3xkk(uint8_t* Vx, uint8_t kk){
    /* 
        The interpreter compares registers Vx to kk, 
        and if they are equal, increments the PC by 2
    */
    if(*Vx == kk)
        cpu.PC += 2;
}

// Skip next instruction if Vx != kk
void inst_4xkk(uint8_t* Vx, uint8_t kk){
    /* 
        The interpreter compares registers Vx to kk, 
        and if they are not equal, increments the PC by 2
    */
    if(*Vx != kk)
        cpu.PC += 2;
}

// Skip next instruction if Vx = Vy
void inst_5xy0(uint8_t* Vx, uint8_t* Vy){
    /*
        The interpreter compares register Vx to register Vy,
        and if they are equal, increments the PC by 2.
    */
    if(*Vx == *Vy)
        cpu.PC += 2;
}

// Set Vx = kk
void inst_6xkk(uint8_t* Vx, uint8_t kk){
    // The interpreter puts the value kk into register Vx
    *Vx = kk;
}

// Set Vx = Vx + kk
void inst_7xkk(uint8_t* Vx, uint8_t kk){
    /* 
        Adds the value kk to the value of register Vx,
        then stores the result in Vx    
    */
    *Vx += kk;
}

// Set Vx = Vy
void inst_8xy0(uint8_t* Vx, uint8_t* Vy){
    // Stores the values of register Vy in register Vx
    *Vx = *Vy;
}

// Set Vx = Vx OR Vy
void inst_8xy1(uint8_t* Vx, uint8_t* Vy){
    /*
        Performs a bitwise OR on the values of Vx and Vy, 
        then stores the result in Vx. 
        A bitwise OR compares the corrseponding bits from two values, 
        and if either bit is 1, then the same bit in the result is also 1. 
        Otherwise, it is 0.
    */
   *Vx |= *Vy;
}

// Set Vx = Vx AND Vy.
void inst_8xy2(uint8_t* Vx, uint8_t* Vy){
    *Vx &= *Vy;
}

// Set Vx = Vx XOR Vy.
void inst_8xy3(uint8_t* Vx, uint8_t* Vy){
    *Vx ^= *Vy;
}

// Set Vx = Vx + Vy, set VF = carry.
void inst_8xy4(uint8_t* Vx, uint8_t* Vy){
    uint val = *Vx + *Vy;
    if((val >> 8) & 0x1)
        cpu.V[0xF] = 1;
    else cpu.V[0xF] = 0;
    *Vx = val & 0xFF;
}

// Set Vx = Vx - Vy, set VF = NOT borrow.
void inst_8xy5(uint8_t* Vx, uint8_t* Vy){
    if(*Vx > *Vy) cpu.V[0xF] = 1;
    else cpu.V[0xF] = 0;
    uint val = *Vx - *Vy;
    *Vx = val & 0xFF;
}

// Set Vx = Vx SHR 1.
void inst_8xy6(uint8_t* Vx, uint8_t* Vy){
    if((*Vx & 0x1) == 1) cpu.V[0xF] = 1;
    else cpu.V[0xF] = 0;
    *Vx = *Vx >> 1;
}

// Set Vx = Vy - Vx, set VF = NOT borrow.
void inst_8xy7(uint8_t* Vx, uint8_t* Vy){
    if(*Vy > *Vx) cpu.V[0xF] = 1;
    else cpu.V[0xF] = 0;
    uint val = *Vy - *Vy;
    *Vx = val & 0xFF;
}

// Set Vx = Vx SHL 1.
void inst_8xyE(uint8_t* Vx, uint8_t* Vy){
    if(((*Vx >> 7) & 0x1) == 1) cpu.V[0xF] = 1;
    else cpu.V[0xF] = 0;
    *Vx = *Vx << 1;
}

// Skip next instruction if Vx != Vy.
void inst_9xy0(uint8_t* Vx, uint8_t* Vy){
    if(*Vx != *Vy) cpu.PC += 2;
}

// Set I = nnn.
void inst_Annn(uint16_t nnn){
    cpu.I = nnn;
}

// Jump to location nnn + V0.
void inst_Bnnn(uint16_t nnn){
    cpu.PC = nnn + cpu.V[0x0];
}

// list of random numbers
int random_list1[] = {445257,907378,123144,543463,234132,856346,342142,525456};
int random_list2[] = {428767,663378,198634,986363,79832,986346,322342,525356};
int random_list3[] = {878957,907378,128644,887634,37132,867768,902142,725456};
// Set Vx = random byte AND kk.
void inst_Cxkk(uint8_t* Vx, uint8_t kk){
    srand(time(NULL));
    srand(random_list1[rand() % sizeof(random_list1)]);
    srand(random_list2[rand() % sizeof(random_list2)]);
    srand(random_list3[rand() % sizeof(random_list3)]);
    srand(random_list1[rand() % sizeof(random_list1)]);
    *Vx = (rand() % 0xFF) & kk;
}

// Display n-byte sprite starting at memory location I at
// (Vx, Vy), set VF = collision
void inst_Dxyn(uint8_t* Vx, uint8_t *Vy, uint8_t n){
    uint8_t byte;
    uint16_t start_addr = cpu.I;

    int x = *Vx;
    int y = *Vy;

    int latch = 0;
    //loop through n bytes
    for(int i=0; i<n; i++){
        byte = memory[start_addr++];
        x = *Vx;
        int xor_stat = -1;
        for(int j=0; j<8; j++){
            
            // wrap around the pixels
            int x1 = x % WIN_WIDTH;
            int y1 = y % WIN_HEIGHT;

            // set or clear pixels
            // if bit is set
            if((byte>>(7-j)) & 0x1){
                uint8_t initial_pixel = pixel_bitmap[x1][y1];
                pixel_bitmap[x1][y1] ^= 1;
                if(initial_pixel ^ pixel_bitmap[x1][y1]) xor_stat = 1;
            }
            // if bit is clear
            else{
                uint8_t initial_pixel = pixel_bitmap[x1][y1];
                pixel_bitmap[x1][y1] ^= 0;
                if(initial_pixel ^ pixel_bitmap[x1][y1]) xor_stat = 1;
            }
            if(xor_stat == 1 && latch == 0) latch = 1;
            x++;
        }
        y++;
    }
    if(latch) cpu.V[0xF] = 1;
    else cpu.V[0xF] = 0;
}

//Skip next instruction if the key with the value of Vx is pressed.
void inst_Ex9E(uint8_t* Vx){
    //Checks the keyboard, and if the key corresponding to the value of
    //Vx is currently in the down position, PC is increase by 2.
    if(keys[*Vx] == 1){
        cpu.PC += 2;
    }
}

//Skip next instruction if the key with the value of Vx is not pressed.
void inst_ExA1(uint8_t* Vx){
    //Checks the keyboard, and if the key corresponding to the value of
    //Vx is currently in the up position, PC is increase by 2.
    if(keys[*Vx] == 0){
        cpu.PC += 2;
    }
}

//Set Vx = delay timer value
void inst_Fx07(uint8_t* Vx){
    *Vx = cpu.DT;
}

// Wait for a key press, store the value of the key in Vx
void inst_Fx0A(uint8_t* Vx){
    while(pressed_key == 0xfe){
        if(terminate_flag == 1){
            return;
        }
    };
    pressed_key = *Vx;
}

// Set delay timer = Vx
void inst_Fx15(uint8_t* Vx){
    // DT is set equal to the value of Vx
    cpu.DT = *Vx;
}

// Set sound timer = Vx
void inst_Fx18(uint8_t* Vx){
    // Set sound timer to the value of Vx
    cpu.ST = *Vx;
}

// Set I = I + Vx
void inst_Fx1E(uint8_t* Vx){
    // The values of I and Vx are added, and the results are stored in I.
    cpu.V[0xF] = ((cpu.I + *Vx) > 0xFFF)? 1:0;
    cpu.I = cpu.I + *Vx;
}

// Set I = location of sprite for digit Vx
void inst_Fx29(uint8_t* Vx){
    /*
        The value of I is set to the location for the hexadeciaml sprite corresponding to the
        value of Vx
    */
   cpu.I = spr_addr[*Vx];
}

// Store BCD representation of Vx in memory locations I, I+1 and I+2.
void inst_Fx33(uint8_t* Vx){
    /*
        The interpreter takes the decimal value of Vx, and places the hunderds
        digit in memory at location in I, the tens digit at location I+1, and the ones digit at
        location I+2.
    */
    uint8_t val = *Vx;
    uint8_t ones = val%10;
    val /= 10;
    uint8_t tens = val%10;
    val /= 10;
    uint8_t hundreds = val%10;

    memory[cpu.I] = hundreds;
    memory[cpu.I + 1] = tens;
    memory[cpu.I + 2] = ones;
}

// Store registers V0 through Vx in memory starting at location I.
void inst_Fx55(uint8_t x){
    for(int i=0; i<=x; i++){
        memory[cpu.I++] = cpu.V[i];
    }
}

// Read registers V0 through Vx in memory starting at location I.
void inst_Fx65(uint8_t x){
    for(int i=0; i<=x; i++){
        cpu.V[i] = memory[cpu.I++];
    }
}