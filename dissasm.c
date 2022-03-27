/*  Read the code into a buffer
    Get a pointer to the beginning of the buffer
    Use the byte at the pointer to determine the opcode
    Print out the name of the opcode using the bytes after the opcode as data, if applicable
    Advance the pointer the number of bytes used by that instruction (1, 2, or 3 bytes)
    If not at the end of the buffer, go to step 3
*/

//e000 2aa2 0c60 0861
/*
   addr op   notes    
   ---- ---- -------------    
   0000 00e0 Clear the screen    
   0002 a220 Set I to address $220    
   0004 6208 V2 = #$08    
   0006 60f8 V0 = #$f8    
   0008 7008 V0 = V0 + #$08    
   000a 6110 V1 = #$10    
*/

/*
Big endian so

PC C1 C2
*/


void Disassemble(unsigned char *codebuffer, int pc){

    //size of opcode
    int opbytes = 2;

    unsigned char *code = &codebuffer[pc];
    unsigned char firstNib = (code[0] >> 4);
    printf("%04x %02x %02x ", pc, code[0], code[1]);
    printf(" - ");


    //big endian
    //bit shifting 6XNN >>4 gives you 0x06

    switch(firstNib){
        case 0x00: 
            {
                printf("0 not handled yet"); 
            }
            break;    
        case 0x01: //JUMP TO NNN ADDR
            {
                pc = code[0] & 0xf;
                int pcsub = code[1];
                printf("%5s $%x%x", "JMP", pc,pcsub);
            }
            break;    
        case 0x02: //CALL subroutine 2NNN
            {
                pc = code[0] & 0xf;
                int pcsub = code[1];
                printf("%5s $%x%x", "CALL", pc,pcsub);
            }
            break;    
        case 0x03: //Cond if(vx == NN) //if = Skip Next instruction //uses VX only
            {
                int NN = code[0] & 0xf;
                printf("JMP if vx == %d", NN); 
            }
            break;    
        case 0x04: // JMP if  vx != NN //uses VX only
            {
                int NN = code[0] & 0xf;                
                printf("JMP if vx != %d", NN); 
            }
            break;    
        case 0x05: //JMP if vx == vy // 5XY0 //compare registers
            {
                int NN = code[0] & 0xf;    
                printf("5 not handled yet");    
            }
            break;
        case 0x06: //Sets VX reg to NN   
            {    
                unsigned char reg = code[0] & 0x0f;
                //AND with 00001111 to get the upper nibble
                //https://stackoverflow.com/questions/12989969/what-does-0x0f-mean-and-what-does-this-code-mean#12990007
                printf("%5s V%01X,#$%02x", "MVI", reg, code[1]);
                     
            }    
            break;    
        case 0x07: //7XNN - Adds NN to VX (Carry Flag Not changed)
            {
                //get register to add into
                unsigned char reg = (code[0] & 0x0f);
                int value = 0;
                printf("%5s V%01X,#$%02x", "ADD", reg, code[1]);
            }
            break; 
        case 0x08: // 9 Cases
            {
                printf("8 not handled yet");    
            }
            break; 
        case 0x09: //9xY0 -- If (Vx != Vy) Skips the next instruction if VX does not equal VY
        //get x
        //get y
        //skip 0
            {
                unsigned char rega = (code[0] & 0x0f);
                unsigned char regb = ((code[1] >> 4) & 0x0f);

                  printf("%5s V%01d V%d", "CND", rega, regb);
            }
            break; 
        case 0x0a: //ANNN - MEM Set I to the Addr NNN
            {    
                unsigned char addresshi = code[0] & 0x0f;
                printf("%5s I,#$%01x%02x", "MVI", addresshi, code[1]);
                 
            }    
            break;    
        case 0x0b: //BNNN - JMP to Index : NNN + V0
            {
                printf("b not handled yet");
            }
            break;
        case 0x0c: //CXNN Rand - Vx = rand() & NN -- Set Vx to the bit& of random number and NN
            {
                printf("c not handled yet");
            }
            break;  
        case 0x0d: //display DXYN - Draw (Vx,Vy, N)
            {
                printf("d not handled yet");
            }
            break;
        case 0x0e:
            {
                printf("e not handled yet");
            }
            break;    
        case 0x0f:
            {
                printf("f not handled yet");
            }
            break;
    }

    
}
