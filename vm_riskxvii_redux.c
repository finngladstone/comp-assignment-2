#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"

/* Key constants */

#define REGISTER_COUNT 32
#define INSTRUCTION_SIZE 512

/* Etc */

#define ARGS uint32_t i, int * registers, int * program_count

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

void parse_TYPE_R_00(ARGS) {
    int * data = code_arr(i);

    switch(data[FUNC3_INDEX]) {
        case 0x0:
        {

        }

        case 0x4:
        {

        }

        case 0x6:
        {

        }

        case 0x7:
        {

        }

        case 0x1:
        {

        }

        case 0x5:
        {

        }

        case 0x2:
        {

        }

        case 0x3:
        {
            
        }
    }
}

void parse_TYPE_R_01(ARGS) {}

void parse_TYPE_I(ARGS) {}

void parse_TYPE_S(ARGS) {}

void parse_TYPE_SB(ARGS) {}

void parse_TYPE_U(ARGS) {}

void parse_TYPE_UJ(ARGS) {}
 
void parse_binary(ARGS) {

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