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


/* Parse opcode */

    // if (opcode == TYPE_I) {
    //     uint8_t byte4 = (i >> (8 * 3)) & 0xff;
    //     printf("Type I: %i\n", byte4);
    // }

    // // get func3 
    //     uint8_t f3_mask = 0x70;
    //     uint8_t byte2 = (i >> (8 * 1)) & 0xff;
    //     uint8_t opcode = f3_mask & byte2;

    //     printf("Mask = %i\n", opcode);

/* */

/* Error blocks */

void func_3_failed() {
    printf("Func3 unrecognised in context\n");
    exit(1);
}

uint8_t get_opcode(uint32_t i) {
    uint8_t byte_1 = i & 0xff;
    uint8_t mask = 0x7F;

    return mask & byte_1;
}

uint8_t get_func3(uint32_t i) {
    uint8_t func3_mask = 0x70;
    uint8_t byte2 = (i >> (8 * 1)) & 0xff;

    return func3_mask & byte2;
}

uint8_t get_func7(uint32_t i) {

}

/* Control flow for parsing binary, 32 bits per iteration */

int parse_binary(uint32_t i) {

    uint8_t opcode = get_opcode(i);
    uint8_t func_3 = get_func3(i);

    switch(opcode)
    {
        
        
        case TYPE_R:
        
        
        
        case TYPE_I:

            if (func_3 == 0x0) {} // addi
            
            else if (func_3 == 0x4) {} // xori

            else if (func_3 == 0x6) {} // ori

            else if (func_3 == 0x7) {} // andi

            else if (func_3 == 0x2) {} // slti

            else if (func_3 == 0x3) {}

            else 
                func_3_failed();

            break;

        
        
        case TYPE_S:

            if (func_3 == 0x0) {} // sb

            else if (func_3 == 0x1) {} // sh

            else if (func_3 == 0x2) {} // sw

            else 
                func_3_failed();

            break;



        
        
        case TYPE_SB:

            if (func_3 == 0x0) {} // beq

            else if (func_3 == 0x1) {} // bne

            else if (func_3 == 0x4) {} // blt

            else if (func_3 == 0x6) {} // bltu

            else if (func_3 == 0x5) {} // bge

            else if (func_3 == 0x7) {}

            else 
                func_3_failed();

            break;



        case TYPE_U:
            break;
        
        case TYPE_UJ:
            break;

        default:
            break;
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

