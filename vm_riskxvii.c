#include "header.h"


/* Malloc / Free VM implementation */

/* Error blocks */

void instruction_invalid(int i, uint32_t * registers, int * program_count) {
    printf("Instruction not implemented - %x\n", i);
    dump_register_banks(registers, *program_count);
    exit(1);
}

void parse_files(char * filename, int * instruction_buffer, unsigned char * byte_buffer) {
    FILE * binaryfile = fopen(filename, "rb");

    if (binaryfile == NULL) {
        printf("Failed to read file: %s\n", filename);
        exit(1);
    }

    if (fread(instruction_buffer, 4, 256, binaryfile)) {}
    rewind(binaryfile);
    if (fread(byte_buffer, 1, 2048, binaryfile)) {}

    fclose(binaryfile);
}


/* Function pointer arrays */

void (*TYPE_R_Pointer[8])(ARGS2) = {add, sll, slt, sltu, xor, srl, or, and};

void (*TYPE_I_Pointer[8])(ARGS2) = {addi, NULL, slti, sltiu, xori, NULL, ori, andi};

void (*TYPE_I_Pointer2[6])(ARGS2) = {lb, lh, lw, NULL, lbu, lhu};

void (*TYPE_S_Pointer[3])(ARGS2) = {sb, sh, sw};

void (*TYPE_SB_Pointer[8])(ARGS2) = {beq, bne, NULL, NULL, blt, bge, bltu, bgeu};

/* FUNCTION ROUTER */
 
void parse_binary(ARGS) {
    struct data codes = { 0 };

    if (update_data_struct(&codes, i) == 0) {
        printf("Instruction Not Implemented: 0x%x\n", i);
        
        dump_register_banks(registers, *program_count);
        exit(1);
    }

    switch(codes.opcode) {

        case TYPE_R:
        {
            if (codes.func7 == 32) {
                if (codes.func3 == 0) 
                    sub(ARGS3);
                else if (codes.func3 == 5) 
                    sra(ARGS3);
                else 
                    instruction_invalid(i, registers, program_count);
            } 
            
            else if (codes.func7 == 0) {
                if (within_range(0, 7, codes.func3))
                    (*TYPE_R_Pointer[codes.func3])(ARGS3);
                else 
                    instruction_invalid(i, registers, program_count);
            } else 
                instruction_invalid(i, registers, program_count);
            
            break;
        }

        case TYPE_I:    
        {
            if (within_range(0, 7, codes.func3) && codes.func3 != 1 && codes.func3 != 5) {
                (*TYPE_I_Pointer[codes.func3])(ARGS3);
            } else {
                instruction_invalid(i, registers, program_count);
            }

            break;
        }

        case TYPE_I_2:
        {
            if (within_range(0, 5, codes.func3) && codes.func3 != 3) {
                (*TYPE_I_Pointer2[codes.func3])(ARGS3);
            } else {
                instruction_invalid(i, registers, program_count);
            }

            break;
        }

        case TYPE_I_3:
        {
            if (codes.func3 == 0x0)
                jalr(ARGS3);
            else
                instruction_invalid(i, registers, program_count);
            
            break;
        }

        case TYPE_S:
        {
            if (within_range(0, 2, codes.func3)) {
                (*TYPE_S_Pointer[codes.func3])(ARGS3);
            } else {
                instruction_invalid(i, registers, program_count);
            }

            break;
        }

        case TYPE_SB:
        {
            
            if (within_range(0, 1, codes.func3) || within_range(4, 7, codes.func3)) {
                (*TYPE_SB_Pointer[codes.func3])(ARGS3);
            } else {
                instruction_invalid(i, registers, program_count);
            }

            break;
        }

        case TYPE_U:
        {
            lui(ARGS3);
            break;
        }

        case TYPE_UJ:
        {
            jal(ARGS3);
            break;
        }

        default:
        {
            instruction_invalid(i, registers, program_count);
            exit(1);
        }
    }
}

int main(int argc, char * argv[]) {
    uint32_t registers[REGISTER_COUNT] = { 0 };
    int program_counter = 0;
    
    // VM instructions
    int instruction_arr[INSTRUCTION_SIZE] = { 0 };
    
    // Byte array of input of first 2048b from .mi file
    unsigned char memory_byte_arr[MEMORY_BYTE_ARR_SIZE] = { 0 }; 
 
    // Update arrs (1 4B array for instructions, 1 1B array for data)
    parse_files(argv[1], instruction_arr, memory_byte_arr);

    while (1) {
        parse_binary(instruction_arr[program_counter/4], registers, &program_counter, memory_byte_arr);
    }

    return 0;
}