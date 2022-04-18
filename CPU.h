#include <cstdint>

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

}CPUstate;

CPUstate* InitializeCPU(void){

    //use calloc to avoid garbage data that malloc would give you
    CPUstate* initState = calloc(sizeof(CPUstate),1);

    initState->MEM = calloc(1024*4,1);
    initState->screen = &(MEM[0xF00]);
    initState->SP = 0xEA0;
    initState->PC = 0x200; //PC always starts at 0x200
}