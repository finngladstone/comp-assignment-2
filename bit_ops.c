#include "header.h"

uint32_t isolate_bits(uint32_t i, int start_index, int n) {
    return (((1 << n) - 1) & (i >> start_index));
}

/* Parsing instruction codes */

uint8_t get_opcode(uint32_t i) {
    // uint8_t byte_1 = i & 0xff;
    // uint8_t mask = 0x7F;

    // return mask & byte_1;

    return isolate_bits(i, 0, 7);
}

uint8_t get_func3(uint32_t i) {

    return isolate_bits(i, 12, 3);
}

uint8_t get_func7(uint32_t i) {
    return isolate_bits(i, 25, 7);
}

uint8_t get_rd(uint32_t i) {
    return isolate_bits(i, 7, 5);
}

int32_t get_imm(uint32_t i) { //TYPE COMPATIBLE

    switch (get_opcode(i)) {

        case TYPE_R:
        {
            return 0;
        }
        
        case TYPE_I: // good
        {
            uint32_t result = isolate_bits(i, 20, 12);
            result = (int32_t) (result << 20) >> 20;
            
            return result;
        }
        case TYPE_I_2:
        {
            uint32_t result = isolate_bits(i, 20, 12);
            result = (int32_t) (result << 20) >> 20;
            
            return result;
        }

        case TYPE_I_3: // good
        {
            uint32_t result = isolate_bits(i, 20, 12);
            result = (int32_t) (result << 20) >> 20;
            
            return result;
        }

        case TYPE_S: // good
        {
            uint32_t result = 0x0;
            result = isolate_bits(i, 7, 5) | result;
            result = isolate_bits(i, 25, 7) << 5 | result;

            result = (int32_t) (result << 20) >> 20;

            return result;
        }

        case TYPE_SB:
        {

            uint32_t result = 0x0;
            result = isolate_bits(i, 7, 1) << 11 | result;
            result = isolate_bits(i, 8, 4) << 1 | result;
            result = isolate_bits(i, 25, 6) << 5 | result;
            result = isolate_bits(i, 31, 1) << 12 | result;

            result = (int32_t) (result << 19) >> 19;

            return result;

        }
            
        case TYPE_U: // good
        {
            uint32_t result = isolate_bits(i, 12, 20);
            result = (int32_t) (result << 20) >> 20;
            
            return result;
        }

        case TYPE_UJ: // good
        
        {
            uint32_t result = 0x0; // 21 digits!

            result = isolate_bits(i, 31, 1) << 20 | result;
            result = isolate_bits(i, 21, 10) << 1 | result;
            result = isolate_bits(i, 20, 1) << 11 | result;
            result = isolate_bits(i, 12, 8) << 12 | result;

            result = (int32_t) (result << 11) >> 11;

            return result;
        }

        default:
            return 0;
    }
} 

uint8_t get_rs_1(uint32_t i) {
    return isolate_bits(i, 15, 5);
}

uint8_t get_rs_2(uint32_t i) {
    return isolate_bits(i, 20, 5);
}


int check_opcode(int opcode) {
    
    switch(opcode) {
        case TYPE_R: 
            return 1;
        case TYPE_I:
            return 1;
        case TYPE_I_2:
            return 1;
        case TYPE_I_3:
            return 1;
        case TYPE_S:
            return 1;
        case TYPE_SB:
            return 1;
        case TYPE_U:
            return 1;
        case TYPE_UJ:
            return 1;
        default:
            return 0;
    }
}

int update_data_struct(struct data * codes, uint32_t i) {
    codes->opcode = get_opcode(i);

    if (check_opcode(codes->opcode) != 1)
        return 0;

    codes->func3 = get_func3(i);
    codes->func7 = get_func7(i);
    
    codes->imm = get_imm(i);
    codes->rd = get_rd(i);

    codes->rs1 = get_rs_1(i);
    codes->rs2 = get_rs_2(i);

    return 1;
}

int within_range(int lower, int upper, int val) {
    if (val >= lower && val <= upper)
        return 1;
    return 0;
}

