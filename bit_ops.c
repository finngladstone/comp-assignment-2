#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* opcode hex values */

#define TYPE_R 0x33
#define TYPE_I 0x13
#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_U 0x37
#define TYPE_UJ 0x6F

/* 
    isolate_bits
    Calculates integer value of certain bits from an integer
    :: uint32_t i :: Integer to gather data from
    :: start_index :: Index from which we start gathering bits (reverse 0-indexed)
    :: n :: number of bits to extract
    Returns desired isolated bits as uint32_t
*/

uint32_t isolate_bits(uint32_t i, int start_index, int n) {
    return (((1 << n) - 1) & (i >> start_index));
}

/* Parsing instruction codes */

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
    return isolate_bits(i, 25, 7);
}

uint8_t get_rd(uint32_t i) {
    // return (((1 << 5) - 1) & (i >> (7))); // how the fuck is this working
    return isolate_bits(i, 7, 5);
}

uint32_t get_imm(uint32_t i) { //TYPE COMPATIBLE

    switch (get_opcode(i)) {
        
        case TYPE_I:
            return isolate_bits(i, 20, 12);

        case TYPE_S:
        {
            uint16_t result = 0x0;
            result = isolate_bits(i, 0, 4) | result;
            result = isolate_bits(i, 25, 7) << 5 | result;

            return result;
        }

        case TYPE_SB:
        {
            uint16_t result = 0x0;
            result = isolate_bits(i, 0, 1) << 11 | result;
            result = isolate_bits(i, 1, 4) | result;
            result = isolate_bits(i, 25, 5) << 5 | result;
            result = isolate_bits(i, 31, 1) << 12 | result;
        }
            
        case TYPE_U:
            return isolate_bits(i, 12, 20);

        case TYPE_UJ:
        
        {
            uint32_t result = 0x0; // 21 digits!

            result = isolate_bits(i, 31, 1) << 20 | result;
            result = isolate_bits(i, 21, 10) << 1 | result;
            result = isolate_bits(i, 20, 1) << 11 | result;
            result = isolate_bits(i, 12, 8) << 12 | result;

            return result;
        }

        default:
            break;
    }
    
    if (get_opcode(i) == TYPE_I) {
        return isolate_bits(i, 20, 12);
    } 
} 

uint8_t get_rs_1(uint32_t i) {
    // return (((1 << 5) - 1) & (i >> (15)));
    return isolate_bits(i, 15, 5);
}

uint8_t get_rs_2(uint32_t i) {
    return isolate_bits(i, 20, 5);
}

int * code_arr(uint32_t i) {
    int reval[7] = 
    {
    get_opcode(i), get_rd(i), get_func3(i),
    get_func7(i), get_rs_1(i), get_rs_2(i),
    get_imm(i)
    };

    return reval;
}