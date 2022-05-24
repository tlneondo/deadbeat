#pragma once

#include <stdlib.h>
#include <stdint.h>

void disp_clear();
void draw(uint8_t Vx, uint8_t Vy, uint8_t N);

typedef struct CPUstate{
    //contains registers
    uint8_t V[15];
    uint16_t I;
    uint16_t SP; //stack point
    uint16_t PC; //program counter
    uint8_t delay;
    uint8_t sound;
    uint8_t *MEM;
    uint8_t *screen;

    //display buffer at 0xF00 in ram
    //stack is at 0xEA0

    uint32_t clockSpeed;
    uint8_t currentClockCnt;
    uint8_t perframeClockLimit;

}CPUstate;

CPUstate* InitializeCPU(void);
uint8_t getKey();
uint8_t getDelay();
void EmulateCh8(unsigned char * codebuffer, CPUstate * state);
void disp_clear(CPUstate * state);
void draw(uint8_t Vx, uint8_t Vy, uint8_t N);
uint8_t decrementTimers(CPUstate * state);
