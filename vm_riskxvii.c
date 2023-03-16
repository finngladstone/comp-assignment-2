#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* opcode hex values */

#define TYPE_R 0x33
#define TYPE_I 0x13
#define TYPE_U 0x37
#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_UJ 0x6F

/* */

void add() {}

void addi() {}

void sub() {}

void lui() {}



/* Control flow for parsing binary, 32 bits per iteration */

int parse_binary(uint32_t i) {
    
    /* Parse opcode */

    uint8_t byte1 = i & 0xff;
    printf("Byte 1 = %i\n", byte1);

    uint8_t mask = 0x7F;
    uint8_t reval = mask & byte1;

    // if (reval == TYPE_R)
    //     ;
    // else if (reval == TYPE_I)
    //     ;
    // else if (reval == TYPE_U)
    //     ;
    // else if (reval == TYPE_S)
    //     ; 
    // else if (reval == TYPE_SB)
    //     ;
    // else if (reval == TYPE_UJ)
    //     ;
    // else {
    //     printf("Invalid opcode\n");
    //     exit(1);
    // }

    if (reval == TYPE_I) {
        uint8_t byte4 = (i >> (8 * 3)) & 0xff;
        printf("Type I: %i\n", byte4);
    }
    
    
}

int main(int argc, char * argv[]) {

    // declare memory here: then pass pointer to fns?

    FILE *myfile;
    myfile = fopen(argv[1], "rb");
    int32_t buffer[32];

    for (int i = 0; i < 32; i++)
        buffer[i] = 0;

    if (myfile == NULL) {
        printf("Error reading file\n");
        exit(1);
    }

    fread(buffer, 1, 32, myfile);

    fclose(myfile);

    for (int i = 0; i < 32; i++) {

        int val = buffer[i];
        int k;

        for (int c = 31; c >= 0; c--) {
            k = val >> c;
            if (k & 1)
                printf("1");
            else  
                printf("0");
        }


        printf(" = %d\n", buffer[i]);
        parse_binary(buffer[i]);
    }

    return 0;
}

