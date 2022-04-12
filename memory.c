#include "memory.h"

#include <stdlib.h>
#include <stdio.h>

uint8_t* memory;
uint16_t stack[16];

const uint8_t start_addr = 0x050;
uint8_t spr_addr[] = {
        start_addr+0,
        start_addr+5,
        start_addr+10,
        start_addr+15,
        start_addr+20,
        start_addr+25,
        start_addr+30,
        start_addr+35,
        start_addr+40,
        start_addr+45,
        start_addr+50,
        start_addr+55,
        start_addr+60,
        start_addr+65,
        start_addr+70,
        start_addr+75,
        start_addr+80 
};

void set_sprite(uint8_t addr, uint8_t B0, uint8_t B1,uint8_t B2,uint8_t B3,uint8_t B4){
    memory[addr++] = B0;
    memory[addr++] = B1;
    memory[addr++] = B2;
    memory[addr++] = B3;
    memory[addr++] = B4;
}

void load_rom(char* filename){
    // load ROM in memory starting at address 0x200;
    FILE *rom = fopen(filename, "rb");
    int size = 0;
    if(rom == NULL) {
        printf("cannot open file: %s\n",filename);
        exit(1);
    }
    fseek(rom, 0, SEEK_END);
    size = ftell(rom);
    fseek(rom, 0, SEEK_SET);
    if(size != fread(memory + 0x200, sizeof(uint8_t), size, rom)){
        printf("error in reading file\n");
        exit(1);
    }
    fclose(rom);
}

char* file_list[] = {
    "roms/SpaceInvaders.ch8",
    "roms/octojam2.ch8",
    "roms/random_number_test.ch8",
    "roms/delay_timer_test.ch8",
    "roms/glitchGhost.ch8",
    "roms/danm8ku.ch8",
    "roms/octoachip8story.ch8",
    "roms/Pong.ch8",
    "roms/test_opcode.ch8",
    "roms/ch8Test.ch8",
    "roms/BC_test.ch8",
    "roms/KeypadTest.ch8",
    "roms/UFO.ch8"
};

void init_memory(char* filename){
    //char* filename = file_list[4];

    memory = (uint8_t*) malloc(sizeof(uint8_t) * 0xFFFF);

    load_rom(filename);

    // set hex sprites at start_address
    //0
    set_sprite(spr_addr[0x0],0xF0,0x90,0x90,0x90,0xF0);
    //1
    set_sprite(spr_addr[0x1],0x20,0x60,0x20,0x20,0x70);
    //2
    set_sprite(spr_addr[0x2],0xF0,0x10,0xF0,0x80,0xF0);
    //3
    set_sprite(spr_addr[0x3],0xF0,0x10,0xF0,0x10,0xF0);
    //4
    set_sprite(spr_addr[0x4],0x90,0x90,0xF0,0x10,0x10);
    //5
    set_sprite(spr_addr[0x5],0xF0,0x80,0xF0,0x10,0xF0);
    //6
    set_sprite(spr_addr[0x6],0xF0,0x80,0xF0,0x90,0xF0);
    //7
    set_sprite(spr_addr[0x7],0xF0,0x10,0x20,0x40,0x40);
    //8
    set_sprite(spr_addr[0x8],0xF0,0x90,0xF0,0x90,0xF0);
    //9
    set_sprite(spr_addr[0x9],0xF0,0x90,0xF0,0x10,0xF0);
    //A
    set_sprite(spr_addr[0xA],0xF0,0x90,0xF0,0x90,0x90);
    //B
    set_sprite(spr_addr[0xB],0xE0,0x90,0xE0,0x90,0xE0);
    //C
    set_sprite(spr_addr[0xC],0xF0,0x80,0x80,0x80,0xF0);
    //D
    set_sprite(spr_addr[0xD],0xE0,0x90,0x90,0x90,0xE0);
    //E
    set_sprite(spr_addr[0xE],0xF0,0x80,0xF0,0x80,0xF0);
    //F
    set_sprite(spr_addr[0xF],0xF0,0x80,0xF0,0x80,0x80);
}