#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"
#include "logic_ops.c"

/* Key constants */

#define REGISTER_COUNT 32
#define INSTRUCTION_SIZE 512

/* Etc */

#define ARGS uint32_t i, int * registers, int * program_count // this is template for binary parse
#define ARGS2 struct data codes, int * registers, int * program_count // this is template for logic / mem ops
#define ARGS3 *codes, registers, &program_count // this gets sent to logic / mem ops

/* Opcode hex values */

#define TYPE_R 0x33
#define TYPE_I 0x13
#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_U 0x37
#define TYPE_UJ 0x6F

/* Error blocks */

void func3_fail() {
    printf("Func3 - undefined behaviour.\n");
    exit(1);
}

void func7_fail() {
    printf("Func7 - undefined behaviour.\n");
    exit(1);
}

void parse_file(char * filename, int * buffer) {
    FILE * binaryfile;

    binaryfile = fopen(filename, "rb");
    if (binaryfile == NULL)
        printf("Failed to read file: %s\n", filename);

    fread(buffer, sizeof(int), 512, binaryfile);
    fclose(binaryfile);

    return;
}

/* Function pointer arrays */

// https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers

void (*TYPE_R_Pointer[8])(ARGS2) = {add, sll, slt, sltu, xor, srl, or, and};
// void (*TYPE_I_Pointer[12])(ARGS) = {addi, xori, ori, andi, }
// void (*TYPE_S_Pointer[3])(ARGS) = {s}
void (*TYPE_SB_Pointer[6])(ARGS2) = {beq, bne, blt, bltu, bge, bgeu};

// add for UJ
// add for type R 010000

/* FUNCTION ROUTER */
 
void parse_binary(ARGS) { 
    struct data * codes;
    update_data_struct(&codes, i);

    switch(codes->opcode) {
        case TYPE_R:
        {
            if (codes->func7 == 32) {
                if (codes->func3 == 0) 
                    sub(ARGS3);
                else if (codes->func3 == 5) 
                    sra(ARGS3);
                else 
                    func3_fail();
            } 
            
            else if (codes->func7 == 0) {
                if (within_range(0, 7, codes->func3))
                    (*TYPE_R_Pointer[codes->func3])(ARGS3);
                else 
                    func3_fail();
            } else 
                func7_fail();
        }


    }
}

int main(int argc, char * argv[]) {
    int registers[REGISTER_COUNT];
    int program_counter;
    int file_instructions[INSTRUCTION_SIZE];

    for (int i = 0; i < REGISTER_COUNT; i++)
        registers[i] = 0;

    program_counter = 0;
 
    parse_file(argv[1], file_instructions);
    for (int i = 0; i < 9; i++)
        parse_binary(file_instructions[i], registers, &program_counter);

    return 0;
}