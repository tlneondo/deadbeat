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

__ is instruction, then followed

*/


void Disassemble(unsigned char *codebuffer, int pc){

    //size of opcode
    int opbytes = 2;

    unsigned char *code = &codebuffer[pc];
    unsigned char firstNib = (code[0] >> 4);
    printf("%04x %02x %02x ", pc, code[0], code[1]);
    printf(" - ");

    switch(firstNib){
        case 0x00: printf("0 not handled yet"); break;    
        case 0x01: printf("1 not handled yet"); break;    
        case 0x02: printf("2 not handled yet"); break;    
        case 0x03: printf("3 not handled yet"); break;    
        case 0x04: printf("4 not handled yet"); break;    
        case 0x05: printf("5 not handled yet"); break;    
        case 0x06:    
            {    
                unsigned char reg = code[0] & 0x0f;    
                printf("%-10s V%01X,#$%02x", "MVI", reg, code[1]);
                     
            }    
            break;    
        case 0x07: printf("7 not handled yet"); break;    
        case 0x08: printf("8 not handled yet"); break;    
        case 0x09: printf("9 not handled yet"); break;    
        case 0x0a:    
            {    
                unsigned char addresshi = code[0] & 0x0f;    
                printf("%-10s I,#$%01x%02x", "MVI", addresshi, code[1]);
                 
            }    
            break;    
        case 0x0b: printf("b not handled yet"); break;    
        case 0x0c: printf("c not handled yet"); break;    
        case 0x0d: printf("d not handled yet"); break;    
        case 0x0e: printf("e not handled yet"); break;    
        case 0x0f: printf("f not handled yet"); break;    
    }

    
}