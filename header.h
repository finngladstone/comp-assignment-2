#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Opcode macros */

#define TYPE_R 0x33
#define TYPE_I 0x13
#define TYPE_I_2 0x3
#define TYPE_I_3 0x67

#define TYPE_S 0x23
#define TYPE_SB 0x63
#define TYPE_U 0x37
#define TYPE_UJ 0x6F

/* Key VM constants */

#define REGISTER_COUNT 32
#define INSTRUCTION_SIZE 256
#define MEMORY_BYTE_ARR_SIZE 2048
#define HEAP_BANK_SIZE 128

/* Function parameters */

#define ARGS uint32_t i, uint32_t * registers, int * program_count, unsigned char * ram // this is template for binary parse
#define ARGS2 struct data codes, uint32_t * registers, int * program_count, unsigned char * ram // this is template for logic / mem ops
#define ARGS3 codes, registers, program_count, ram // this gets sent to logic / mem ops
#define LOGIC_OP_ARGS struct data codes, uint32_t * registers, int * program_count, uint8_t * ram

struct data {
    int opcode;
    
    int func3;
    int func7;

    int rd;
    int imm;

    int rs1;
    int rs2;
};

/* bit_ops function declarations */

uint32_t isolate_bits(uint32_t i, int start_index, int n);

uint8_t get_opcode(uint32_t i);

uint8_t get_func3(uint32_t i);

uint8_t get_func7(uint32_t i);

uint8_t get_rd(uint32_t i);

int32_t get_imm(uint32_t i);

uint8_t get_rs_1(uint32_t i);

uint8_t get_rs_2(uint32_t i);

int check_opcode(int opcode);

int update_data_struct(struct data * codes, uint32_t i);

int within_range(int lower, int upper, int val);


/* logic_ops function declarations */

void dump_register_banks(uint32_t * registers, int program_counter);

void add(LOGIC_OP_ARGS);

void addi(LOGIC_OP_ARGS);

void sub(LOGIC_OP_ARGS);

void lui(LOGIC_OP_ARGS);

void xor(LOGIC_OP_ARGS);

void xori(LOGIC_OP_ARGS);

void or(LOGIC_OP_ARGS);

void ori(LOGIC_OP_ARGS);

void and(LOGIC_OP_ARGS);

void andi(LOGIC_OP_ARGS);

void sll(LOGIC_OP_ARGS);

void srl(LOGIC_OP_ARGS);

void sra(LOGIC_OP_ARGS);

void lb(LOGIC_OP_ARGS);

void lh(LOGIC_OP_ARGS);

void lw(LOGIC_OP_ARGS);

void lbu(LOGIC_OP_ARGS);

void lhu(LOGIC_OP_ARGS);

void sb(LOGIC_OP_ARGS);

void sh(LOGIC_OP_ARGS);

void sw(LOGIC_OP_ARGS);

void slt(LOGIC_OP_ARGS);

void slti(LOGIC_OP_ARGS);

void sltu(LOGIC_OP_ARGS);

void sltiu(LOGIC_OP_ARGS);

void beq(LOGIC_OP_ARGS);

void bne(LOGIC_OP_ARGS);

void blt(LOGIC_OP_ARGS);

void bltu(LOGIC_OP_ARGS);

void bge(LOGIC_OP_ARGS);

void bgeu(LOGIC_OP_ARGS);

void jal(LOGIC_OP_ARGS);

void jalr(LOGIC_OP_ARGS);

#endif