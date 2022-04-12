#include <stdint.h>

#define WIN_WIDTH 64
#define WIN_HEIGHT 32

#define PIX_SIZE 16

#define BGCOL 0xFF000066
#define FGCOL 0xFFFF3399

#define DISP_RATE 16
#define TIMER_RATE 0
#define CPU_RATE 100

extern uint8_t pixel_bitmap[WIN_WIDTH][WIN_HEIGHT];
extern uint32_t *sdl_pixels;

extern uint8_t keys[16];
extern uint8_t pressed_key;

extern uint8_t terminate_flag;