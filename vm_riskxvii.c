#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* opcode hex values */

#define TYPE_R 0x33
#define TYPE_I 0x13
#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_U 0x37
#define TYPE_UJ 0x6F

#define REGISTER_NO 32

/* Helper functions */

int raise(int x, int power) {
    int reval = 1;  
    for (int i = 1; i <= power; i++)
        reval *= x;

    return reval;
}

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



/* Virtual routines */

void console_write_char(int i) {
    printf("%c\n", i);
}

void console_write_signed(int i) {}

void console_write_unsigned(int i) {}

void halt() {}

void console_read_char() {}

void console_read_signed() {}

void dump() {}

void dump_registers() {}

void dump_mem() {}

void (*virtual_routines[])() = 
{
    console_write_char, console_write_signed, console_write_unsigned,
    halt, console_read_char, console_read_signed, dump, dump_registers, 
    dump_mem
};


/* Error blocks */

void func_3_failed() {
    printf("Func3 unrecognised in context\n");
    exit(1);
}

/* Parsing the instruction codes */

uint32_t isolate_bits(uint32_t i, int start_index, int n) {
    return (((1 << n) - 1) & (i >> start_index));
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
    return isolate_bits(i, 25, 7);
}

uint8_t get_rd(uint32_t i) {
    // return (((1 << 5) - 1) & (i >> (7))); // how the fuck is this working
    return isolate_bits(i, 7, 5);
}

uint16_t get_imm(uint32_t i) { //TYPE COMPATIBLE
    
    if (get_opcode(i) == TYPE_I) {
        // return (((1 << 12) - 1) & (i >> (20)));
        return isolate_bits(i, 20, 12);
    }
} 

uint8_t get_rs_1(uint32_t i) {
    // return (((1 << 5) - 1) & (i >> (15)));
    return isolate_bits(i, 15, 5);
}

uint8_t get_rs_2(uint32_t i) {}


/* Control flow for parsing binary, 32 bits per iteration */

int parse_binary(uint32_t i, int * registers, int * p_counter) {

    uint8_t opcode = get_opcode(i);
    uint8_t func_3 = get_func3(i);

    switch(opcode)
    {
        
        
        case TYPE_R:
        {
            int rs_1 = get_rs_1(i);
            int rs_2 = get_rs_2(i);

            int func_3 = get_func3(i);
            int func_7 = get_func7(i);

            int rd = get_rd(i);

            switch(func_7) {
                case 0:
                {
                    if (func_3 == 0x0) {
                        // add
                    } 

                    else if (func_3 == 0x4) {
                        // xor
                    }

                    else if (func_3 == 0x6) {
                        // or 
                    }

                    else if (func_3 == 0x7) {
                        // and 
                    } 

                    else if (func_3 == 0x1) {
                        // sll
                    }

                    else if (func_3 == 0x5) {
                        // srl
                    }

                    else if (func_3 == 0x2) {
                        // slt
                    }

                    else if (func_3 == 0x3) {
                        // sltu
                    }

                    else {
                        func_3_failed();
                    }
                }

                case 0x20:
                {
                    
                }
            }
            
        }
        case TYPE_I:   
        {
            int rd = get_rd(i);
            int imm = get_imm(i);
            int rs_1 = get_rs_1(i);

            if (rd == 0) {
                // do nothing
            }

            if (func_3 == 0x0) { 
                // ADDI - R[rd] = R[rs1] + imm
                registers[rd] = registers[rs_1] + imm;
            } 
            
            else if (func_3 == 0x4) {
                // xori 0 RD = RS1 ^ imm
                registers[rd] = raise(registers[rs_1], imm);
            } 

            else if (func_3 == 0x6) {
                // ORI - rd = rs1 | imm
                registers[rd] = registers[rs_1] | imm;
            } 

            else if (func_3 == 0x7) {
                // ANDI - rd = rs1 & imm
                registers[rd] = registers[rs_1] & imm;
            } 

            else if (func_3 == 0x2) {
                // slti - RD = (RS1 < imm) ? 1 : 0

                if (registers[rs_1] < imm)
                    registers[rd] = 1;
                else 
                    registers[rd] = 0;
            }

            else if (func_3 == 0x3) {
                // NO SIGNED / UNSIGNED IMPLEMNETATION YET
                // sltiu - RD = (RS1 < imm) ? 1 : 0

                if (registers[rs_1] < imm)
                    registers[rd] = 1;
                else 
                    registers[rd] = 0;
            }

            else 
                func_3_failed();

            break;

        }

        case TYPE_S:

            if (func_3 == 0x0) {
                // sb M[R[rs1] + imm] = R[rs2]
            } 

            else if (func_3 == 0x1) {} // sh

            else if (func_3 == 0x2) {} // sw

            else 
                func_3_failed();

            break;



        
        
        case TYPE_SB:

            if (func_3 == 0x0) {
                // BEQ
                // if rs1 == rs2 then PC = PC + (imm << 1)
            } 

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

    /* Setup registers */

    int registers[REGISTER_NO];
    int program_counter;

    for (int i = 0; i < REGISTER_NO; i++) {
        registers[i] = 0;
    }

    program_counter = 0;    


    /* FILE IO*/
    
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

        parse_binary(buffer[i], registers, &program_counter);

        // int val = buffer[i];
        // int k;

        // for (int c = 31; c >= 0; c--) {
        //     k = val >> c;
        //     if (k & 1)
        //         printf("1");
        //     else  
        //         printf("0");
        // }


        // printf(" = %d\n", buffer[i]);
        
    }

    return 0;
}

