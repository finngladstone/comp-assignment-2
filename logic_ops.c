#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"

/* Default arguments*/

#define LOGIC_OP_ARGS struct data codes, int * registers, int * program_count

void add(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] + registers[codes.rs2];

    program_count += 4;
}

void addi(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] + codes.imm;

    program_count += 4;
}

void sub(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] - registers[codes.rs2];

    program_count += 4;
}

void lui(LOGIC_OP_ARGS) {
    int32_t i = 0;
    i = i | (codes.imm << 11);
    registers[codes.rd] = i;

    program_count += 4;
}

void xor(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] ^ registers[codes.rs2];

    program_count += 4;
}

void xori(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] ^ codes.imm;

    program_count += 4;
}

void or(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] | registers[codes.rs2];

    program_count += 4;
}

void ori(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] | codes.imm;

    program_count += 4;
}

void and(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] & registers[codes.rs2];

    program_count += 4;
}

void andi(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] & codes.imm;

    program_count += 4;
}

void sll(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] << registers[codes.rs2];

    program_count += 4;
}

void srl(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] >> registers[codes.rs2];

    program_count += 4;
}

void sra(LOGIC_OP_ARGS) {
    registers[codes.rd] = registers[codes.rs1] >> registers[codes.rs2];

    program_count += 4;
}

/* Memory access operations */

void lb(LOGIC_OP_ARGS) {}

void lh(LOGIC_OP_ARGS) {}

void lw(LOGIC_OP_ARGS) {}

void lbu(LOGIC_OP_ARGS) {}

void lhu(LOGIC_OP_ARGS) {}

void sb(LOGIC_OP_ARGS) {}

void sh(LOGIC_OP_ARGS) {}

void sw(LOGIC_OP_ARGS) {}

/* Program flow operations */

void slt(LOGIC_OP_ARGS) {
    registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    program_count += 4;
}

void slti(LOGIC_OP_ARGS) {
    registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    program_count += 4;
}

void sltu(LOGIC_OP_ARGS) { // unsigned
    registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    program_count += 4;
}

void sltiu(LOGIC_OP_ARGS) { // unsigned
    registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    program_count += 4;
}

void beq(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] == registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void bne(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] != registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void blt(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] < registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void bltu(LOGIC_OP_ARGS) { // unsigned
    
}

void bge(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] >= registers[codes.rs2])
        program_count += (codes.imm << 1);
}

void bgeu(LOGIC_OP_ARGS) {} // unsigned

void jal(LOGIC_OP_ARGS) {

    registers[codes.rd] = *program_count + 4;
    program_count += (codes.imm << 1);
}

void jalr(LOGIC_OP_ARGS) {
    registers[codes.rd] = *program_count + 4;
    program_count = &(registers[codes.rs1]) + codes.imm;
}
