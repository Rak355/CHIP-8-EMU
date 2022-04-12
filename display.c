#include "display.h"
#include "memory.h"
#include "sdl_main.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

void clear_screen(){
    for(int i=0; i<WIN_WIDTH; i++){
        for(int j=0; j<WIN_HEIGHT; j++){
            pixel_bitmap[i][j] = 0;
        }
    }
}

void paint_pixel(uint x, uint y, uint32_t color){
    for(int i=0; i<PIX_SIZE; i++){
        for(int j=0; j<PIX_SIZE; j++){
            sdl_pixels[ i+(PIX_SIZE*x) + (j+PIX_SIZE*y) * (PIX_SIZE * WIN_WIDTH) ] = color;
        }
    }
}

void draw_screen(){
    for(int i=0; i<WIN_WIDTH; i++){
        for(int j=0; j<WIN_HEIGHT; j++){
            if(pixel_bitmap[i][j] == 1) 
                paint_pixel(i,j,FGCOL);
            else if(pixel_bitmap[i][j] == 0) 
                paint_pixel(i,j,BGCOL);
            else{
                printf("ERROR: draw screen, invalid bitmap");
                exit(0);
            }
        }
    }
}