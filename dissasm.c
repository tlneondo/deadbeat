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
    unsigned char secNib = (code[1]);
    printf("%04x %02x %02x ", pc, code[0], code[1]);
    printf(" - ");


    //big endian
    //bit shifting 6XNN >>4 gives you 0x06

    switch(firstNib){
        case 0x00: 
            {
                switch(secNib)
                {
                    case 0xE0:
                    {
                        printf("Display Clear - disp_clear() Call");
                    }

                    case 0xEE:
                    {
                        printf("Return");
                    }
                }
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
        case 0x08: // 9 Cases, involing X&Y registers
            {
                unsigned char regX = code[0] & 0x0f;
                unsigned char regY = (code[1] & 0xf0) >> 4;
                unsigned char optionNum = (code[1] & 0x0f);
                switch(optionNum)
                {
                    case 0x00: // set vx to value in vy
                        printf("%6s - V%d to V%d", "Assign", regX, regY);
                        break;
                    case 0x01: //vx = (VX | VY)
                        printf("%6s - V%d = V%d OR V%d", "OR", regX, regX, regY);
                        break;                    
                    case 0x02: //vx = (VX & VY)
                        printf("%6s - V%d = V%d AND V%d", "AND", regX, regX, regY);                    
                        break;
                    case 0x03: //vx = (VX XOR VY)
                    printf("%6s - V%d = V%d XOR V%d", "XOR", regX, regX, regY);
                        break;
                    case 0x04: //VX += VY, VF set to 1 if carry
                    printf("%s - V%d = V%d += V%d", "PLUSEQ - Const", regX, regX, regY);
                        break;
                    case 0x05: //VX -= VY, VF = 0 when borrow, 1 when not
                    printf("%s - V%d = V%d -= V%d", "MINUSEQ - Const", regX, regX, regY);
                        break;
                    case 0x06: //VX >>= 1, send LSB to VF, then shift VX right by 1
                    printf("%s - V%d >> 1", "SHIFTRIGHT", regX);
                        break;
                    case 0x07: //vx = Vy - Vx, VF = 0 when borrow, 1 if not
                    printf("%s - V%d = V%d - V%d", "EQUALSDIFF", regX, regY, regX);                    
                        break;
                    case 0x0E: //Vx <<= 1, store MSB of VX in VF, then LShift VX by 1
                        printf("%s - V%d >> 1", "SHIFTLEFT", regX);
                        break;
                }
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
                unsigned int addr = 0;
                addr |= (addresshi << 8);
                addr |= code[1];
                printf("%5s I,0x%02x", "MVI", addr);
                 
            }    
            break;    
        case 0x0b: //BNNN - JMP to Index : NNN + V0
            {
                unsigned char jmpOffseta = ( (code[0] & 0x0f));
                unsigned char jmpOffsetb = (code[1]);
                unsigned char * addressJMP = malloc(sizeof(unsigned char) * 2);
                addressJMP[0] = jmpOffseta;
                addressJMP[1] = jmpOffsetb;
                unsigned int addrJMP = 0;
                addrJMP |= (jmpOffseta << 8);
                addrJMP |= jmpOffsetb;

                printf("%4s PC = V0 + 0x%02x", "FLOW", addrJMP);
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
        case 0x0e: // 2 cases
            {
                printf("e not handled yet");
            }
            break;    
        case 0x0f: // 9 cases
            {
                printf("f not handled yet");
            }
            break;
    }

    
}
