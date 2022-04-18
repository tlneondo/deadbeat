typedef struct CPUstate{
    //contains registers
    unsigned char * v[15];
    unsigned char * I;
    unsigned char * SP;
    unsigned char * MEM[4000];
}