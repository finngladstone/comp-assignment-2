#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "logic_ops.c"

/* Key constants */

#define REGISTER_COUNT 32
#define INSTRUCTION_SIZE 256
#define MEMORY_BYTE_ARR_SIZE 2048

/* Etc */

#define ARGS uint32_t i, uint32_t * registers, int * program_count, unsigned char * ram // this is template for binary parse
#define ARGS2 struct data codes, uint32_t * registers, int * program_count, unsigned char * ram // this is template for logic / mem ops
#define ARGS3 codes, registers, program_count, ram // this gets sent to logic / mem ops

/* Opcode hex values */

#define TYPE_R 0x33

#define TYPE_I 0x13
#define TYPE_I_2 0x3
#define TYPE_I_3 0x67

#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_U 0x37
#define TYPE_UJ 0x6F

void printbits(int x)
{
    for(int i=sizeof(x)<<3; i; i--)
        putchar('0'+((x>>(i-1))&1));
}

/* Error blocks */

void instruction_invalid(int i) {
    printf("Instruction not implemented - %x\n", i);
}

void func3_fail(int func3) {
    printf("Invalid func3 - %i\n", func3);
    exit(1);
}

void func7_fail() {
    printf("Func7 - undefined behaviour.\n");
    exit(1);
}

void opcode_fail() {
    printf("Opcode - undefined behaviour.\n");
    exit(1);
}

void parse_file(char * filename, int * buffer, int size) {
    FILE * binaryfile;

    binaryfile = fopen(filename, "rb");
    if (binaryfile == NULL) {
        printf("Failed to read file: %s\n", filename);
        exit(1);
    }

    if (fread(buffer, sizeof(buffer[0]), size, binaryfile)) {}

    fclose(binaryfile);

    return;
}

void parse_file_single_byte(char * filename, unsigned char * buffer, int size) {
    FILE * binaryfile;

    binaryfile = fopen(filename, "rb");
    if (binaryfile == NULL) {
        printf("Failed to read file: %s\n", filename);
        exit(1);
    }

    if (fread(buffer, sizeof(buffer[0]), size, binaryfile)) {}

    fclose(binaryfile);

    return;
}


/* Function pointer arrays */

// https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers

void (*TYPE_R_Pointer[8])(ARGS2) = {add, sll, slt, sltu, xor, srl, or, and};

void (*TYPE_I_Pointer[7])(ARGS2) = {addi, NULL, slti, sltiu, xori, ori, andi};

void (*TYPE_I_Pointer2[6])(ARGS2) = {lb, lh, lw, NULL, lbu, lhu};

void (*TYPE_S_Pointer[3])(ARGS2) = {sb, sh, sw};

void (*TYPE_SB_Pointer[8])(ARGS2) = {beq, bne, NULL, NULL, blt, bge, bltu, bgeu};

/* FUNCTION ROUTER */
 
void parse_binary(ARGS) { 
    // printf("%i\n", i);
    struct data codes = { 0 };
    
    int opcode_valid = update_data_struct(&codes, i);
    
    if (opcode_valid == 0) {
        printf("Instruction Not Implemented: 0x%x\n", i);
        
        dump_register_banks(registers, *program_count);
        exit(1);
    }

    // printf("Opcode = %x, func3 = %i, func7 = %i, RD = %i, rs1 = %i, rs2 = %i, imm = %i\n", 
    //     codes.opcode, codes.func3, codes.func7, codes.rd, codes.rs1, codes.rs2, codes.imm);

    switch(codes.opcode) {

        case TYPE_R:
        {
            if (codes.func7 == 32) {
                if (codes.func3 == 0) 
                    sub(ARGS3);
                else if (codes.func3 == 5) 
                    sra(ARGS3);
                else 
                    func3_fail(codes.func3);
            } 
            
            else if (codes.func7 == 0) {
                if (within_range(0, 7, codes.func3))
                    (*TYPE_R_Pointer[codes.func3])(ARGS3);
                else 
                    func3_fail(codes.func3);
            } else 
                func7_fail();
            
            break;
        }

        case TYPE_I:    
        {
            if (within_range(0, 6, codes.func3) && codes.func3 != 1) {
                (*TYPE_I_Pointer[codes.func3])(ARGS3);
            } else {
                func3_fail(codes.func3);
            }

            break;
        }

        case TYPE_I_2:
        {
            if (within_range(0, 5, codes.func3) && codes.func3 != 3) {
                (*TYPE_I_Pointer2[codes.func3])(ARGS3);
            } else {
                func3_fail(codes.func3);
            }

            break;
        }

        case TYPE_I_3:
        {
            if (codes.func3 == 0x0)
                jalr(ARGS3);
            else
                func3_fail(codes.func3);
            
            break;
        }

        case TYPE_S:
        {
            if (within_range(0, 2, codes.func3)) {
                (*TYPE_S_Pointer[codes.func3])(ARGS3);
            } else {
                func3_fail(codes.func3);
            }

            break;
        }

        case TYPE_SB:
        {
            
            if (within_range(0, 1, codes.func3) || within_range(4, 7, codes.func3)) {
                (*TYPE_SB_Pointer[codes.func3])(ARGS3);
            } else {
                func3_fail(codes.func3);
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
            printf("This should not occur\n");
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

    program_counter = 0;
 
    parse_file(argv[1], instruction_arr, INSTRUCTION_SIZE);
    parse_file_single_byte(argv[1], memory_byte_arr, MEMORY_BYTE_ARR_SIZE);
    

    while (1) {
        // printf("PC = %i, ", program_counter);
        parse_binary(instruction_arr[program_counter/4], registers, &program_counter, memory_byte_arr);
        // printf(", Register[1] = %i\n", registers[1]);
    }

    return 0;
}

/* Testing code */

// printf("Press ENTER key to Continue\n");  
        // getchar();

 // for (int i = 0; i < 32; i++) {
        //     printf("R[%i] = %i\n", i, registers[i]);
        // }

    

        