#include <stdint.h>

void disp_clear();

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
    initState->screen = &(initState->MEM[0xF00]);
    initState->SP = 0xEA0;
    initState->PC = 0x200; //PC always starts at 0x200

    return initState;
}

void EmulateCh8(unsigned char * codebuffer, CPUstate * state){

    int opbytes = 2;

    unsigned char * code = &codebuffer[state->PC];

    //easy guide
    unsigned char A = (code[0] >> 4); //first nibble of first byte - also the letter of the Opcode
    unsigned char B = (code[0] & 0x0f);
    unsigned char C = (code[1] >> 4);
    unsigned char D = (code[1] & 0x0f);

    ///firNibA|secNibA////firNibB|SecNibB

    switch(code[0] >> 4){

        case 0x00:{ //extras and display clears

            switch(code[1]){ //switch on whole second byte
            
                case 0xE0:{ //display clear
                    disp_clear(state);
                break;
                }

                case 0xEE:{ //return
                break;
                }

                default: {
                break;
                }
                
            }

        }
        break;
        case 0x01:{ //JMP to
            uint8_t * dest = calloc(2, sizeof(uint8_t));

            dest[0] = (code[0] & 0x0f);
            dest[1] = code[1];

            state->PC = 0; //clear 
            state->PC |= *dest;  

            free(dest);        

        }
        break;
        case 0x02:{ //Call subroutine

            
        }
        break;
        case 0x03:{ //Skip if VX = NN

            if(state->V[(code[0] & 0x0f)] == code[1]){
                state->PC += 2;
            }

        }
        break;
        case 0x04:{ //Skip if VX != NN

           if(state->V[(code[0] & 0x0f)] != code[1]){
                state->PC += 2;
            }        

        }
        break;
        case 0x05:{ //skip if VX & VY are equal

           if(state->V[(code[0] & 0x0f)] == state->V[code[1] >> 4]){
                state->PC += 2;
            }        

        }
        break;
        case 0x06:{ //set VX to NN
            state->V[(code[0] & 0x0f)] = code[1];
        }
        break;
        case 0x07:{ //Add NN to VX (No Carry)
            state->V[(code[0] & 0x0f)] += code[1];
        }
        break;
        case 0x08:{ //Operations on Two Registers

            uint8_t regX = (code[0] & 0x0f);
            uint8_t regY = (code[1] >> 4);
            uint8_t optionNum = (code[1] & 0x0f);

            switch(optionNum){
                case 0x00:{//set vx to value in vy
                    state->V[regX] = state->V[regY];
                }
                break;
                case 0x01:{ //vx = (VX | VY)
                    state->V[regX] = (state->V[regX] | state->V[regY]);
                }
                break;
                case 0x02:{ //AND
                    state->V[regX] = (state->V[regX] & state->V[regY]);
                }
                break;
                case 0x03:{ //XOR
                    state->V[regX] = (state->V[regX] ^ state->V[regY]);
                }
                break;
                case 0x04:{ //add with carry
                            //https://iq.opengenus.org/addition-using-bitwise-operations/
                            //Carry = a & b
                            //sum = a ^ b
                            // uint8_t sum = 0;
                            // uint8_t carry = 0;

                            // do{

                            //     carry = state->V[regX] & state->V[regY];
                            //     sum = state->V[regX] ^ state->V[regY];

                            //     state->V[regX] = sum;
                            //     state->V[15] = (carry << 1);
                                
                            // }while(state->V[regY] != 0);

                            //carry
                            state->V[15] = state->V[regX] & state->V[regY];
                            //sum
                            state->V[regX] = state->V[regX] ^ state->V[regY];
                }
                break;
                case 0x05:{ //subtract with borrow

                            //borrow
                            state->V[15] = ~(state->V[regX]) & state->V[regY];
                            //sum
                            state->V[regX] = state->V[regX] ^ state->V[regY];        
                }
                break;
                case 0x06:{ //right shift, store in VF
                    state->V[15] = (state->V[regX] & 0x01);
                    state->V[regX] >>= 1;
                }
                break;
                case 0x07:{}
                break;
                case 0x0E:{}
                break;
                
                



            }

        }
        break;
        case 0x09:{}
        break;
        case 0x0a:{}
        break;
        case 0x0b:{}
        break;
        case 0x0c:{}
        break;
        case 0x0d:{}
        break;
        case 0x0e:{}
        break;
        case 0x0f:{}
        break;
        default:
        break;
    }
        
        
}




void disp_clear(CPUstate * state){

    //clear display memory, not implememnted yet


    return;
}