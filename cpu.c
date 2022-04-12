#include "cpu.h"
#include "memory.h"
#include "instructions.h"
#include "sdl_main.h"

#include <stdio.h>
#include <unistd.h>

struct Cpu cpu;

void init_cpu(){
    cpu.PC = 0x200;
    cpu.SP = -1;
    cpu.I = 0;

    cpu.DT = 0;
    cpu.ST = 0;

    for(int i=0; i<16; i++){
        cpu.V[i] = 0;
    }
}

char V_dbg[][3] = {"V0\0","V1\0","V2\0","V3\0","V4\0","V5\0","V6\0","V7\0",
"V8\0","V9\0","VA\0","VB\0","VC\0","VD\0","VE\0","VF\0"};

void timer_cycle(){
    while(terminate_flag != 1){
        if(cpu.DT > 0) cpu.DT--;
        if(cpu.ST > 0) cpu.ST--;
    }
    usleep(TIMER_RATE);
}

void cpu_cycle(char *filename){
    init_memory(filename);
    init_cpu();

    FILE* log_file = fopen("log.txt","w");

    int file_dbg = 0;
    int print_dbg = 1;

    while(terminate_flag != 1){
        uint16_t prev_PC = cpu.PC;

        uint8_t opcode_high = memory[cpu.PC++];
        uint8_t opcode_low = memory[cpu.PC++];

        uint8_t nibble1 = (opcode_high >> 4) & 0xF;
        uint8_t nibble2 = (opcode_high >> 0) & 0xF;
        uint8_t nibble3 = (opcode_low >> 4) & 0xF;
        uint8_t nibble4 = (opcode_low >> 0) & 0xF;

        //printf("[PC:%x],%x,%x,%x,%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
        //fprintf(log_file, "[PC:%x],%x,%x,%x,%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
        
        if(nibble1 == 0x0 && nibble2 == 0x0 && nibble3 == 0xE && nibble4 == 0x0){
            if(file_dbg) fprintf(log_file,"[%x] CLS :%x%x%x%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] CLS :%x%x%x%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
            inst_00E0();
        }
        else if(nibble1 == 0x0 && nibble2 == 0x0 && nibble3 == 0xE && nibble4 == 0xE){
            if(file_dbg) fprintf(log_file,"[%x] RET :%x%x%x%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] RET :%x%x%x%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
            inst_00EE();
        }
        else if(nibble1 == 0x0 && (nibble3 !=0xE && (nibble4 != 0x0 || nibble4 != 0xE))){
            uint16_t nnn = 0x0000;
            nnn |= nibble2 << 8;
            nnn |= nibble3 << 4;
            nnn |= nibble4;
            nnn &= 0xFFFF;
            if(file_dbg) fprintf(log_file,"[%x] SYS %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SYS %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            inst_0nnn(nnn);
        }
        else if(nibble1 == 0x1){
            uint16_t nnn = 0;
            nnn |= nibble2 << 8;
            nnn |= nibble3 << 4;
            nnn |= nibble4;
            nnn &= 0xFFFF;
            if(file_dbg) fprintf(log_file,"[%x] JP %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] JP %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            inst_1nnn(nnn);
        }
        else if(nibble1 == 0x2){
            uint16_t nnn = 0;
            nnn |= nibble2 << 8;
            nnn |= nibble3 << 4;
            nnn |= nibble4;
            nnn &= 0xFFFF;
            if(file_dbg) fprintf(log_file,"[%x] CALL %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] CALL %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            inst_2nnn(nnn);
        }
        else if(nibble1 == 0x3){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t kk = 0;
            kk |= nibble3 << 4;
            kk |= nibble4;
            kk &= 0xFF;
            if(file_dbg) fprintf(log_file,"[%x] SE %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SE %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            inst_3xkk(x,kk);
        }
        else if(nibble1 == 0x4){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t kk = 0;
            kk |= nibble3 << 4;
            kk |= nibble4;
            kk &= 0xFF;
            if(file_dbg) fprintf(log_file,"[%x] SNE %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SNE %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            inst_4xkk(x,kk);
        }
        else if(nibble1 == 0x5 && nibble4 == 0x0){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] SE %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SE %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_5xy0(x,y);
        }
        else if(nibble1 == 0x6){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t kk = 0;
            kk |= nibble3 << 4;
            kk |= nibble4;
            kk &= 0xFF;
            if(file_dbg) fprintf(log_file,"[%x] LD %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            inst_6xkk(x,kk);
        }
        else if(nibble1 == 0x7){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t kk = 0;
            kk |= nibble3 << 4;
            kk |= nibble4;
            kk &= 0xFF;
            if(file_dbg) fprintf(log_file,"[%x] ADD %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] ADD %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            inst_7xkk(x,kk);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x0){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] LD %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy0(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x1){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] OR %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] OR %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy1(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x2){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] AND %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] AND %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy2(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x3){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] XOR %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] XOR %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy3(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x4){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] ADD %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] ADD %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy4(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x5){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] SUB %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SUB %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy5(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x6){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] SHR %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SHR %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy6(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0x7){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] SUBN %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SUBN %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xy7(x,y);
        }
        else if(nibble1 == 0x8 && nibble4 == 0xE){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] SHL %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SHL %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_8xyE(x,y);
        }
        else if(nibble1 == 0x9 && nibble4 == 0x0){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            if(file_dbg) fprintf(log_file,"[%x] SNE %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SNE %s, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], nibble1, nibble2, nibble3, nibble4);
            inst_9xy0(x,y);
        }
        else if(nibble1 == 0xA){
            uint16_t nnn = 0;
            nnn |= nibble2 << 8;
            nnn |= nibble3 << 4;
            nnn |= nibble4;
            nnn &= 0xFFFF;
            if(file_dbg) fprintf(log_file,"[%x] LD %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD I, %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            inst_Annn(nnn);
        }
        else if(nibble1 == 0xB){
            uint16_t nnn = 0;
            nnn |= nibble2 << 8;
            nnn |= nibble3 << 4;
            nnn |= nibble4;
            nnn &= 0xFFFF;
            if(file_dbg) fprintf(log_file,"[%x] JP %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] JP V0, %x :%x%x%x%x\n", prev_PC, nnn, nibble1, nibble2, nibble3, nibble4);
            inst_Bnnn(nnn);
        }
        else if(nibble1 == 0xC){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t kk = 0xFF;
            kk |= nibble3 << 4;
            kk |= nibble4;
            if(file_dbg) fprintf(log_file,"[%x] RND %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] RND %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], kk, nibble1, nibble2, nibble3, nibble4);
            inst_Cxkk(x,kk);
        }
        else if(nibble1 == 0xD){
            uint8_t* x = &cpu.V[nibble2];
            uint8_t* y = &cpu.V[nibble3];
            uint8_t n = nibble4;
            if(file_dbg) fprintf(log_file,"[%x] DRW %s, %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], n, nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] DRW %s, %s, %x :%x%x%x%x\n", prev_PC, V_dbg[nibble2], V_dbg[nibble3], n, nibble1, nibble2, nibble3, nibble4);
            inst_Dxyn(x,y,n); 
        }
        else if(nibble1 == 0xE && nibble3 == 0x9 && nibble4 == 0xE){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] SKP %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SKP %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Ex9E(x);
        }
        else if(nibble1 == 0xE && nibble3 == 0xA && nibble4 == 0x1){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] SKNP %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] SKNP %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_ExA1(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x0 && nibble4 == 0x7){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] LD %s, DT :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD %s, DT :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx07(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x0 && nibble4 == 0xA){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] LD %s, K :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD %s, K :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx0A(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x1 && nibble4 == 0x5){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] LD DT, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD DT, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx15(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x1 && nibble4 == 0x8){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] LD ST, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD ST, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx18(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x1 && nibble4 == 0xE){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] ADD I, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] ADD I, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx1E(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x2 && nibble4 == 0x9){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] LD F, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD F, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx29(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x3 && nibble4 == 0x3){
            uint8_t* x = &cpu.V[nibble2];
            if(file_dbg) fprintf(log_file,"[%x] LD B, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD B, %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx33(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x5 && nibble4 == 0x5){
            uint8_t x = nibble2;
            if(file_dbg) fprintf(log_file,"[%x] LD [I], %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD [I], %s :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx55(x);
        }
        else if(nibble1 == 0xF && nibble3 == 0x6 && nibble4 == 0x5){
            uint8_t x = nibble2;
            if(file_dbg) fprintf(log_file,"[%x] LD %s, [I] :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            if(print_dbg) printf("[%x] LD %s, [I] :%x%x%x%x\n", prev_PC, V_dbg[nibble2], nibble1, nibble2, nibble3, nibble4);
            inst_Fx65(x);
        }
        else{
            if(print_dbg) printf("incorrect instruction:[PC:%x],%x,%x,%x,%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
            if(file_dbg) fprintf(log_file, "incorrect instruction:[PC:%x],%x,%x,%x,%x\n", prev_PC, nibble1, nibble2, nibble3, nibble4);
        }

        if(file_dbg) fprintf(log_file, "V0:%x,V1:%x,V2:%x,V3:%x,V4:%x,V5:%x,V6:%x,V7:%x,V8:%x,V9:%x,VA:%x,VB:%x,VC:%x,VD:%x,VE:%x,VF:%x,I:%x\n",
        cpu.V[0],cpu.V[1],cpu.V[2],cpu.V[3],cpu.V[4],cpu.V[5],cpu.V[6],cpu.V[7],cpu.V[8],cpu.V[9],cpu.V[0xA],cpu.V[0xB],cpu.V[0xC],cpu.V[0xD],cpu.V[0xE],cpu.V[0xF],cpu.I);
        if(print_dbg) printf("V0:%x,V1:%x,V2:%x,V3:%x,V4:%x,V5:%x,V6:%x,V7:%x,V8:%x,V9:%x,VA:%x,VB:%x,VC:%x,VD:%x,VE:%x,VF:%x,I:%x\n",
        cpu.V[0],cpu.V[1],cpu.V[2],cpu.V[3],cpu.V[4],cpu.V[5],cpu.V[6],cpu.V[7],cpu.V[8],cpu.V[9],cpu.V[0xA],cpu.V[0xB],cpu.V[0xC],cpu.V[0xD],cpu.V[0xE],cpu.V[0xF],cpu.I);


        if(file_dbg) fprintf(log_file, "Stack: top->[%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x]\n",
        stack[0],stack[1],stack[2],stack[3],stack[4],stack[5],stack[6],stack[7],stack[8],stack[9],stack[0xA],stack[0xB],stack[0xC],stack[0xD],stack[0xE],stack[0xF]);
        if(print_dbg) printf("Stack: top->[%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x]\n",
        stack[0],stack[1],stack[2],stack[3],stack[4],stack[5],stack[6],stack[7],stack[8],stack[9],stack[0xA],stack[0xB],stack[0xC],stack[0xD],stack[0xE],stack[0xF]);
    
        usleep(CPU_RATE);
    }

    fclose(log_file);
}