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
#include "dissasm.c"
#include "CPU.c"

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
    //initialize program counter
    //chip 8 starts at 0x200
    //we skip ahead then read backwards
    ch8CPU->PC = 0x200;
    ///printf("%4s %2s %2s", "PC", "C0", "C1\n");
    while (ch8CPU->PC < inputRom->fSize +0x200){

        //read input



        //process current instruction
        EmulateCh8(inputRom->romData, ch8CPU);
        //increment program counter
        ch8CPU->PC += 2;

        //decrement timers
        decrementTimers(ch8CPU);
    }        
}


//#################################################################
//#################################################################
//#################################################################
//#################################################################


int main(int argc, char** argv){

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
        case 1:{ //run file
            romPack* inputRom;
            inputRom = openROM(argv);
            runEmu(inputRom);
        }
        break;   
        case 2: { //run file with dissasm
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
