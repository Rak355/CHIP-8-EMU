#include "sdl_main.h"
#include "cpu.h"
#include "display.h"
//<--todo remove
#include "memory.h"
#include "instructions.h"
//-->
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <SDL2/SDL.h>

int display_width = WIN_WIDTH * PIX_SIZE;
int display_height = WIN_HEIGHT * PIX_SIZE;

uint8_t keys[16];
uint8_t pressed_key;
uint8_t terminate_flag;

uint32_t *sdl_pixels;
uint8_t pixel_bitmap[WIN_WIDTH][WIN_HEIGHT];

void check_kb(SDL_Event event){
    if (event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
            case SDLK_0:
                pressed_key = 0x0;
                keys[0x0] = 1;
                //return 0x0;
                break;
            case SDLK_1:
                pressed_key = 0x1;
                keys[0x1] = 1;
                //return 0x1;
                break;
            case SDLK_2:
                pressed_key = 0x2;
                keys[0x2] = 1;
                //return 0x2;
                break;
            case SDLK_3:
                pressed_key = 0x3;
                keys[0x3] = 1;
                //return 0x3;
                break;
            case SDLK_4:
                pressed_key = 0x4;
                keys[0x4] = 1;
                //return 0x4;
                break;
            case SDLK_5:
                pressed_key = 0x5;
                keys[0x5] = 1;
                //return 0x5;
                break;
            case SDLK_6:
                pressed_key = 0x6;
                keys[0x6] = 1;
                //return 0x6;
                break;
            case SDLK_7:
                pressed_key = 0x7;
                keys[0x7] = 1;
                //return 0x7;
                break;
            case SDLK_8:
                pressed_key = 0x8;
                keys[0x8] = 1;
                //return 0x8;
                break;
            case SDLK_9:
                pressed_key = 0x9;
                keys[0x9] = 1;
                //return 0x9;
                break;
            case SDLK_a:
                pressed_key = 0xa;
                keys[0xa] = 1;
                //return 0xa;
                break;
            case SDLK_b:
                pressed_key = 0xb;
                keys[0xb] = 1;
                //return 0xb;
                break;
            case SDLK_c:
                pressed_key = 0xc;
                keys[0xc] = 1;
                //return 0xc;
                break;
            case SDLK_d:
                pressed_key = 0xd;
                keys[0xd] = 1;
                //return 0xd;
                break;
            case SDLK_e:
                pressed_key = 0xe;
                keys[0xe] = 1;
                //return 0xe;
                break;
            case SDLK_f:
                pressed_key = 0xf;
                keys[0xf] = 1;
                //return 0xf;
                break;    
            }
    }
    if (event.type == SDL_KEYUP){
        pressed_key = 0xfe;
        switch(event.key.keysym.sym){
            case SDLK_0:
                //pressed_key = 0x0;
                keys[0x0] = 0;
                //return 0x0;
                break;
            case SDLK_1:
                //pressed_key = 0x1;
                keys[0x1] = 0;
                //return 0x1;
                break;
            case SDLK_2:
                //pressed_key = 0x2;
                keys[0x2] = 0;
                //return 0x2;
                break;
            case SDLK_3:
                //pressed_key = 0x3;
                keys[0x3] = 0;
                //return 0x3;
                break;
            case SDLK_4:
                //pressed_key = 0x4;
                keys[0x4] = 0;
                //return 0x4;
                break;
            case SDLK_5:
                //pressed_key = 0x5;
                keys[0x5] = 0;
                //return 0x5;
                break;
            case SDLK_6:
                //pressed_key = 0x6;
                keys[0x6] = 0;
                //return 0x6;
                break;
            case SDLK_7:
                //pressed_key = 0x7;
                keys[0x7] = 0;
                //return 0x7;
                break;
            case SDLK_8:
                //pressed_key = 0x8;
                keys[0x8] = 0;
                //return 0x8;
                break;
            case SDLK_9:
                //pressed_key = 0x9;
                keys[0x9] = 0;
                //return 0x9;
                break;
            case SDLK_a:
                //pressed_key = 0xa;
                keys[0xa] = 0;
                //return 0xa;
                break;
            case SDLK_b:
                //pressed_key = 0xb;
                keys[0xb] = 0;
                //return 0xb;
                break;
            case SDLK_c:
                //pressed_key = 0xc;
                keys[0xc] = 0;
                //return 0xc;
                break;
            case SDLK_d:
                //pressed_key = 0xd;
                keys[0xd] = 0;
                //return 0xd;
                break;
            case SDLK_e:
                //pressed_key = 0xe;
                keys[0xe] = 0;
                //return 0xe;
                break;
            case SDLK_f:
                //pressed_key = 0xf;
                keys[0xf] = 0;
                //return 0xf;
                break;    
            }
    }
}

void sdl_cycle(){
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *framebuffer;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(display_width, display_height, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    sdl_pixels = (uint32_t*) malloc(sizeof(uint32_t) * display_width * display_height);
    framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, display_width, display_height);

    while(1){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            terminate_flag = 1;
            printf("exiting, bye\n");
            break;
        }
        check_kb(event);
        draw_screen();

        SDL_UpdateTexture(framebuffer, NULL, sdl_pixels, display_width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    usleep(DISP_RATE);
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("ERROR, incorrect argument: main <filename>\n");
        exit(1);
    }
    pthread_t cpu_thread, sdl_thread, timer_thread;  
    int p1 = pthread_create(&cpu_thread, NULL, (void*)&cpu_cycle, argv[1]);
    int p2 = pthread_create(&sdl_thread, NULL, (void*)&sdl_cycle, NULL);
    int p3 = pthread_create(&timer_thread, NULL, (void*)&timer_cycle, NULL);

    printf("creation:%d,%d,%d\n", p1, p2, p3);

    p3 = pthread_join(timer_thread, NULL);
    p2 = pthread_join(cpu_thread, NULL);
    p1 = pthread_join(sdl_thread, NULL);

    printf("join:%d,%d,%d\n", p1, p2, p3);
    
   return 0;
}