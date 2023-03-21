#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"

/* Key constants */

#define REGISTER_COUNT 32
#define INSTRUCTION_SIZE 512

/* Opcode hex values */

#define TYPE_R 0x33
#define TYPE_I 0x13
#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_U 0x37
#define TYPE_UJ 0x6F

/* Helping with binary parsing */

#define OPCODE_INDEX 0
#define RD_INDEX 1
#define FUNC3_INDEX 2
#define FUNC7_INDEX 3
#define RS1_INDEX 4
#define RS2_INDEX 5
#define IMM_INDEX 6

void parse_file(char * filename, int * buffer) {
    FILE * binaryfile;

    binaryfile = fopen(filename, "rb");
    if (binaryfile == NULL)
        printf("Failed to read file: %s\n", filename);

    fread(buffer, sizeof(int), 512, binaryfile);
    fclose(binaryfile);

    return;
}

void parse_TYPE_R();

void parse_binary(uint32_t i, int * registers, int * program_count) {
    // int * data = code_arr(i);

    switch(get_opcode(i))
    {
        case TYPE_R:
        {

        }

        case TYPE_I:
        {

        }

        case TYPE_S:
        {

        }

        case TYPE_SB:
        {

        }
        
        case TYPE_U:
        {

        }

        case TYPE_UJ:
        {

        }

        default:
            break;
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