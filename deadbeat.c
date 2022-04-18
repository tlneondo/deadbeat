//deadbeat is a chip8 emulator
//its called deadbeat because that how i feel sometimes
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

/*
void runDissasm(unsigned char* buf, int fileSize){
    //initialize program counter
    //chip 8 starts at 0x200
    //we skip ahead then read backwards
    int pc = 0x200;
    printf("%4s %2s %2s", "PC", "C0", "C1\n");
    while (pc < fileSize+0x200){

        //go through each line and assign information to each instruction
        Disassemble(buf, pc);
        //increment to next instruction
        pc+=2;
        printf("\n");
    }
}
*/



int main(int argc, char**argv){

    FILE *f= fopen(argv[1], "rb");    
    if (f==NULL){    
        printf("error: Couldn't open %s\n", argv[1]);    
        exit(1);    
    }    

    //Get the file size and read it into a memory buffer    
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);    
    fseek(f, 0L, SEEK_SET);

    //create buffer of size 
    unsigned char *fileInputBuffer = malloc(fsize + 0x200);

    //close buffer properly
    fread(fileInputBuffer +0x200, fsize, 1, f);    
    fclose(f); 
    
    //dissasm func
    //runDissasm(fileInputBuffer, fsize);

    CPUstate* ch8CPU = InitializeCPU();

    EmulateCh8(fileInputBuffer, ch8CPU);


    //initialize program counter
    //chip 8 starts at 0x200
    //we skip ahead then read backwards
    int pc = 0x200;
    ///printf("%4s %2s %2s", "PC", "C0", "C1\n");
    while (pc < fsize+0x200){

        //go through each line and assign information to each instruction
        EmulateCh8(fileInputBuffer, ch8CPU);
        //increment to next instruction
        pc+=2;
        printf("\n");
    }    

    return 0;
            
}