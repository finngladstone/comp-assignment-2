#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "logic_ops.c"

/* Key constants */

#define REGISTER_COUNT 32
#define MEMORY_SIZE 512

/* Etc */

#define ARGS uint32_t i, int * registers, int * program_count, int * ram // this is template for binary parse
#define ARGS2 struct data codes, int * registers, int * program_count, int * ram // this is template for logic / mem ops
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

void func3_fail(int func3) {
    printf("Func3 %i - undefined behaviour.\n", func3);
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

void parse_file(char * filename, int * buffer) {
    FILE * binaryfile;

    binaryfile = fopen(filename, "rb");
    if (binaryfile == NULL)
        printf("Failed to read file: %s\n", filename);

    fread(buffer, sizeof(int), MEMORY_SIZE, binaryfile);
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
    update_data_struct(&codes, i);

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
            opcode_fail();
            break;
        }
    }
}

int main(int argc, char * argv[]) {
    int registers[REGISTER_COUNT];
    int program_counter;
    int memory_image[MEMORY_SIZE];

    for (int i = 0; i < REGISTER_COUNT; i++)
        registers[i] = 0;

    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory_image[i] = 0;
    }

    program_counter = 0;
 
    parse_file(argv[1], memory_image);

    while (1) {

        // for (int i = 0; i < 32; i++) {
        //     printf("R[%i] = %i\n", i, registers[i]);
        // }

        // printf("PC = %i, ", program_counter);
        parse_binary(memory_image[program_counter/4], registers, &program_counter, memory_image);
        // printf("Press ENTER key to Continue\n");  
        // getchar(); 
    }

    return 0;
}