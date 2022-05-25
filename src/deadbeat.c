//deadbeat is a chip8 emulator
//made with the help of emudev & bisqwit


//steps
/*
1 - To get familiar with the instruction set
35 instructions
2 bytes each
opcode in the first nibble

2 - Have a tool to use to examine programs
3 - End up with code that we can use in the emulator for debugging
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "dissasm.h"
#include "CPU.h"
#include <SDL2/SDL.h>

typedef struct romPack{
    unsigned char* romData;
    int fSize;
}romPack;

// romPack* openROM(char** argvIn);
// void runDissasm(romPack* inputRom){
// void runEmu(romPack* inputRom);


romPack* openROM(char** argvIn){

    FILE *f= fopen(argvIn[1], "rb");    
    if (f==NULL){    
        printf("error: Couldn't open %s\n", argvIn[1]);    
        exit(1);    
    }

    romPack* inRom = malloc(sizeof(romPack));

    //Get the file size and read it into a memory buffer    
    fseek(f, 0L, SEEK_END);    
    inRom->fSize = ftell(f);    
    fseek(f, 0L, SEEK_SET);

    //create buffer of size 
    inRom->romData = malloc(inRom->fSize + 0x200);

    //close buffer properly
    fread(inRom->romData +0x200, inRom->fSize, 1, f);    
    fclose(f);

    return inRom;
}

void runDissasm(romPack * inputRom){
    //initialize program counter
    //chip 8 starts at 0x200
    //we skip ahead then read backwards
    int pc = 0x200;
    printf("%4s %2s %2s", "PC", "C0", "C1\n");
    while (pc < inputRom->fSize +0x200){

        //go through each line and assign information to each instruction
        Disassemble(inputRom->romData, pc);
        //increment to next instruction
        pc+=2;
        printf("\n");
    }
}

void runEmu(romPack * inputRom){
    CPUstate* ch8CPU = InitializeCPU();
    int emuRunning = 1;

    printf("Current PC Location: \n");
    while(emuRunning){

        //printf("%x %d --  \n", ch8CPU->PC, ch8CPU->PC);
        
        //proces instruction
        EmulateCh8(inputRom->romData, ch8CPU);

        //check inputs

        //update screen every 8 operations
        if(ch8CPU->currentClockCnt == 8){
            //update screen
            ch8CPU->currentClockCnt = 0; //reset
        }

        //update timers


        //iterate the program counter
        ch8CPU->PC += 2;
        
        char c;
        scanf("%c",&c);

        //catch end of rom
    }
    return;
}


//#################################################################
//#################################################################
//#################################################################
//#################################################################

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600


int main(int argc, char** argv){

    // //SDL SETUP
    // if(SDL_Init(SDL_INIT_EVERYTHING) != 0 ){
    //     printf("Error Initializing SDL: %s\n", SDL_GetError());
    // }

    // SDL_Window* screen = SDL_CreateWindow("DeadBeat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE_X, SCREEN_SIZE_Y, SDL_WINDOW_SHOWN);

    // if(!screen){

    //     fprintf(stderr, "error creating window.\n");
    //     return 2;
    // }





    //default is run
    //da for dissasemble

    if(argc > 2){
        printf("error: Too Many Arguments\n");
        return -1;
    }

    switch(argc){
        case 0: {
            printf("error: Too Few Arguments\n");
            return -2;
        }
        break;
        case 2:{ //run file
            romPack* inputRom;
            inputRom = openROM(argv);
            runEmu(inputRom);
        }
        break;   
        case 3: { //run file with dissasm
            romPack* inputRom;
            inputRom = openROM(argv);
            runDissasm(inputRom);
        }
        break;   
        default:{

        }
        break;
    }

    return 0;
            
}
