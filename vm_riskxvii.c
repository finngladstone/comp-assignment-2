#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TYPE_R 00110011b
#define TYPE_I 0x13
#define TYPE_U 00110111b
#define TYPE_S 00100011b
#define TYPE_SB 01100011b
#define TYPE_UJ 01101111b


/* 
    Each line has 32 bits
    Make a header file with all required imports! (?)

*/


// could potentially employ structs to make parsing easier
struct op {

};

int parse_binary(uint32_t i) {
    
    // int x = (number >> (8 * n)) & 0xff;
    // such that n = 0 --> first byte, etc
    // read back to front however!
    // | - 2nd - | - 1st - |
    // 1111 1111 0000 0000 

    uint8_t byte1 = i & 0xff;
    printf("Byte 1 = %i\n", byte1);

    uint8_t mask = 0x7F;
    uint8_t reval = mask & byte1;

    if (reval == TYPE_I)
        printf("Yes cunt u did it\n");
    else 
        printf("Die\n");
    
    
}

int main(int argc, char * argv[]) {
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

