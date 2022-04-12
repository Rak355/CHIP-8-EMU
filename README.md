# CHIP-8-EMU
A simple emulator for CHIP-8 interpreter 

# Requirements
1. Unix environment
2. SDL2
3. make
4. C compiler, preferably gcc.

# How to build
1. On console run<br> 
   `make`
2. To run the program<br>
   `main.o <filename>`

# Bugs
1. Various chip-8 programs have various assummption regarding some of the instructions. So this implementation might not work for some of the programs.
2. Instruction implementation is not clean. Timing issues are present.
3. Display flickering is present. If the program draws sprites after clearing the screen, flicker will occur due to XOR of pixels.
4. Input is not clean.

# Warning
1. All illustrated CHIP-8 programs in this file have copyrights of the respective authors.
2. This program is still in development. Some of the instructions needs corrections.

# Screenshots
## Pong (2 Players)<br>
![pong](https://i.imgur.com/Zv7NXDg.png)

## OctoJam2 (animation)<br>
![octojam](https://i.imgur.com/ZNGO9nx.png)
