#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"

/* Helping with binary parsing */

#define OPCODE_INDEX 0
#define RD_INDEX 1
#define FUNC3_INDEX 2
#define FUNC7_INDEX 3
#define RS1_INDEX 4
#define RS2_INDEX 5
#define IMM_INDEX 6

/* Default arguments*/

#define ARGS struct data codes, int * registers, int * program_count

void add(ARGS) {
    registers[codes.rd] = registers[codes.rs1] + registers[codes.rs2];

    program_count += 1;
}

void addi(ARGS) {
    registers[codes.rd] = registers[codes.rs1] + codes.imm;

    program_count += 1;
}

void sub(ARGS) {
    registers[codes.rd] = registers[codes.rs1] - registers[codes.rs2];

    program_count += 1;
}

void lui(ARGS) {
    int32_t i = 0;
    i = i | (codes.imm << 11);
    registers[codes.rd] = i;

    program_count += 1;
}

void xor(ARGS) {
    registers[codes.rd] = registers[codes.rs1] ^ registers[codes.rs2];

    program_count += 1;
}

void xori(ARGS) {
    registers[codes.rd] = registers[codes.rs1] ^ codes.imm;

    program_count += 1;
}

void or(ARGS) {
    registers[codes.rd] = registers[codes.rs1] | registers[codes.rs2];

    program_count += 1;
}

void ori(ARGS) {
    registers[codes.rd] = registers[codes.rs1] | codes.imm;

    program_count += 1;
}

void and(ARGS) {
    registers[codes.rd] = registers[codes.rs1] & registers[codes.rs2];

    program_count += 1;
}

void andi(ARGS) {
    registers[codes.rd] = registers[codes.rs1] & codes.imm;

    program_count += 1;
}

void sll(ARGS) {
    registers[codes.rd] = registers[codes.rs1] << registers[codes.rs2];

    program_count += 1;
}

void srl(ARGS) {
    registers[codes.rd] = registers[codes.rs1] >> registers[codes.rs2];

    program_count += 1;
}

void sra(ARGS) {
    registers[codes.rd] = registers[codes.rs1] >> registers[codes.rs2];

    program_count += 1;
}

/* Program flow operations */

void slt(ARGS) {
    registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    program_count += 1;
}

void slti(ARGS) {
    registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    program_count += 1;
}

void sltu(ARGS) { // unsigned
    registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    program_count += 1;
}

void sltiu(ARGS) { // unsigned
    registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    program_count += 1;
}

void beq(ARGS) {
    
    if (registers[codes.rs1] == registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void bne(ARGS) {
    
    if (registers[codes.rs1] != registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void blt(ARGS) {
    
    if (registers[codes.rs1] < registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void bltu(ARGS) { // unsigned
    
}

void bge(ARGS) {
    
    if (registers[codes.rs1] >= registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void bgeu(ARGS) {} // unsigned

void jal(ARGS) {

    registers[codes.rd] = program_count + 4;
    program_count += (codes.imm << 1);
}

void jalr(ARGS) {
    registers[codes.rd] = program_count + 4;
    program_count = registers[codes.rs1] + codes.imm;
}
