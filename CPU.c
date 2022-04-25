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

}CPUstate;

CPUstate* InitializeCPU(void);
uint8_t getKey();
uint8_t getDelay();
void EmulateCh8(unsigned char * codebuffer, CPUstate * state);
void disp_clear(CPUstate * state);
void draw(uint8_t Vx, uint8_t Vy, uint8_t N);


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
                    //carry
                    state->V[15] = state->V[regX] & state->V[regY];
                    //sum
                    state->V[regX] = state->V[regX] ^ state->V[regY];
                }
                break;
                case 0x05:{ //subtract with borrow
                    //borrow
                    state->V[15] = ~(state->V[regX]) & state->V[regY];
                    //difference
                    state->V[regX] = state->V[regX] ^ state->V[regY];        
                }
                break;
                case 0x06:{ //right shift, store in VF
                    state->V[15] = (state->V[regX] & 0x01);
                    state->V[regX] >>= 1;
                }
                break;
                case 0x07:{ //Vx = Vy - Vx 	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not. 
                    state->V[15] = state->V[regY] & state->V[regX];
                    state->V[regX] = state->V[regY] ^ state->V[regX];

                }
                break;
                case 0x0E:{ //left shift, store MSB in vF
                    state->V[15] = (state->V[regX] & 64) >> 7; //10000000 in bin
                    state->V[regX] <<= 1;
                }
                break;
            }
        }
        break;
        case 0x09:{ //skip if vx != vy
            uint8_t regX = (code[0] & 0x0f);
            uint8_t regY = (code[1] >> 4);

            if(state->V[regX] != state->V[regY]){
                state->PC += 1;
            }
        }
        break;
        case 0x0a:{ /// Set I to a memory address
            uint8_t * dest = calloc(2, sizeof(uint8_t));
            dest[0] = (code[0] & 0x0f);
            dest[1] = code[1];

            state->I = 0; //clear 
            state->I |= *dest;  

            free(dest);   
        }
        break;
        case 0x0b:{ //Jump to NNN + V0
            uint8_t * dest = calloc(2, sizeof(uint8_t));
            dest[0] = (code[0] & 0x0f);
            dest[1] = code[1];
            dest += state->V[0];

            state->PC = 0; //clear 
            state->PC |= *dest;  

            free(dest);
        }
        break;
        case 0x0c:{ //Vx= rand() & NN
            uint8_t regX = (code[0] & 0x0f);
            state->V[regX] = code[1] & rand();
        }
        break;
        case 0x0d:{ //draw at XY, with height - push out to other function
            uint8_t cordX = (code[0] & 0x0f);
            uint8_t cordY = (code[1] >> 4);
            uint8_t height = (code[1] & 0xf0);
            draw(cordX, cordY,height);
        }
        break;
        case 0x0e:{
            uint8_t reg = (code[0] & 0x0f);
            switch(code[1]){ //switch on whole second byte
                case 0x9E:{ //if(key() == Vx)
                    if(state->V[reg] == getKey()){
                        state->PC += 1;
                    }
                }
                break;
                case 0xA1:{ //if(key() != Vx)
                    if(state->V[reg] != getKey()){
                        state->PC += 1;
                    }                
                }
                break;
                default: {
                }
                break;
            }
        }
        break;
        case 0x0f:{
            uint8_t reg = (code[0] & 0x0f);
            switch(code[1]){ //switch on whole second byte
                case 0x07:{ // set reg to value of delay
                    state->V[reg] = getDelay();
                }
                break;
                case 0x0A:{ // getKey
                    state->V[reg] = getKey();
                }
                break;
                case 0x15:{ // set delay timer
                    state->delay = state->V[reg];
                }
                break;
                case 0x18:{ // set sound timer
                    state->sound = state->V[reg];
                }
                break;
                case 0x1E:{ // adds Vx to I, no change in VF
                    state->I += state->V[reg];
                }
                break;
                case 0x29:{ // I = sprite_addr[Vx]
                    //dummied out for now
                }
                break;
                case 0x33:{ // BCD storing in Vx
                    /*BCD from http://emulator101.com/
                        42 - 101010
                        32 + 8 + 2
                        Opcode FX33 converts the number in register X into 3 Binary-coded decimal (BCD) digits,storing them into the memory location pointed to by the I register. Humans like to read base-10 numbers, and this instruction is convenient to convert hex numbers to base-10 for display. Here is one possible way to do it
                    */
                    uint8_t ones, tens, hundreds;
                    uint8_t value = state->V[reg];
                    ones = value % 10;
                    value = value / 10;
                    tens = value % 10;
                    hundreds = value / 10;
                    state->MEM[state->I] = hundreds;
                    state->MEM[state->I+1] = tens;
                    state->MEM[state->I+2] = ones;
                }
                break;
                case 0x55:{ // dump V0-VX inclusive 
                    //dummied out for now
                }
                break;
                case 0x65:{ // load
                    //dummied out for now
                }
                break;
                default: {
                }
                break;
            }            



        }
        break;
        default:
        break;
    }
        
        
}




void disp_clear(CPUstate * state){

    //clear display memory, not implememnted yet


    return;
}

void draw(uint8_t Vx, uint8_t Vy, uint8_t N){
    return;
}

CPUstate* InitializeCPU(void){

    //use calloc to avoid garbage data that malloc would give you
    CPUstate* initState = calloc(sizeof(CPUstate),1);

    initState->MEM = calloc(1024*4,1);
    initState->screen = &(initState->MEM[0xF00]);
    initState->SP = 0xEA0;
    initState->PC = 0x200; //PC always starts at 0x200

    return initState;
}

uint8_t getKey(){
    //get key from stdin?
    //turn into 8bit int
    uint8_t inputKey;

    return inputKey;
}

uint8_t getDelay(){
    uint8_t delay;

    return delay;
}

uint8_t decrementTimers(CPUstate * state){
    uint8_t status = 0;


    return status;
}
