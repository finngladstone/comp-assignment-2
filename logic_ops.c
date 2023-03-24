#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"

void memory_address_invalid(int address) {
    printf("Address %i invalid.\n", address);
    exit(1);
}

/* Default arguments*/

#define LOGIC_OP_ARGS struct data codes, int * registers, int * program_count, uint8_t * ram

/* Virtual routines */

void console_write_character(int ascii) {
    printf("%c", ascii);
}

void console_write_signed_int(int value) {
    printf("%i", value);
}

void console_write_unsigned_int(int value) {
    printf("%u", value);
}

void halt() {
    printf("CPU Halt Requested\n");
    exit(0);
}

int console_read_char() {
    int c = getchar();
    return c;
}

int console_read_signed_int() {
    int c;

    if (scanf("%d", &c)) {}

    return c;
}

void dump_pc(int pc) {
    printf("%x", pc);
}

void dump_register_banks(int * registers, int program_counter) {
    printf("PC = 0x%08x;\n", program_counter);
    
    for (int i = 0; i < 32; i++) {
        printf("R[%i] = 0x%08x;\n", i, registers[i]);
    }
}

void dump_memory_word(int value) {
    printf("%x", value);
}


/* Function routers for memory-based virtual routines */

void write_to_memory(int address, int value, int program_counter, int * registers, unsigned char * ram, int bytes) {
    // printf("write called, addr = %i, value = %i\n", address, value);
    
    switch (address) {
        case 0x0800:
        {
            console_write_character(value);
            break;
        }

        case 0x0804:
        {
            console_write_signed_int(value);
            break;
        }

        case 0x0808:
        {
            console_write_unsigned_int(value);
            break;
        }

        case 0x080C:
        {
            halt();
            break;
        }

        case 0x0820:
        {
            dump_pc(program_counter);
            break;
        }

        case 0x0824:
        {
            dump_register_banks(registers, program_counter);
            break;
        }

        case 0x0828:
        {
            dump_memory_word(value); // fucked
            break;
        }

        default:
        {
            if (within_range(1025, 2047, address)) {

                int j = 0;
                for (int i = address + bytes - 1; i >= address; i--) {
                    
                    int byte = (uint8_t) isolate_bits(value, j, 8);
                    ram[i] = byte;

                    j+=8;
                }

                
            } else {
                memory_address_invalid(address);
            }

            break;
        }
    }
}

int read_from_memory(int address, uint8_t * ram, int bytes) {
    // printf("READ CALLED, addr = %i\n", address);

    switch(address) {
        case 0x0812:
        {
            return console_read_char();
        }

        case 0x0816:
        {
            return console_read_signed_int();
        }

        default:
        {
            if (within_range(0, 2047, address)) {
                int reval = 0;
                int shift = 0;
                
                for (int i = address + bytes - 1; i >= address; i--) {
                    reval = (reval | ram[i] << shift);
                    shift+=8;
                }

                return reval;
            } else {
                memory_address_invalid(address);
            }
        }
    }

    printf("Failed\n");
    exit(1);
}



/* Logic operataions */

void add(LOGIC_OP_ARGS) { 

    //printf("addi R[%i] = R[%i] + R[%i]\n", codes.rd, codes.rs1, codes.rs2);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] + registers[codes.rs2];

    *program_count += 4;
}

void addi(LOGIC_OP_ARGS) {

    //printf("addi R[%i] = R[%i] + R[%i]\n", codes.rd, codes.rs1, codes.imm);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] + codes.imm;

    *program_count += 4;
}

void sub(LOGIC_OP_ARGS) {

    //printf("sub R[%i] = R[%i] - R[%i]\n", codes.rd, codes.rs1, codes.rs2);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] - registers[codes.rs2];

    *program_count += 4;
}

void lui(LOGIC_OP_ARGS) {

    //printf("lui R[%i] = %i << 12\n", codes.rd, codes.imm);

    int32_t i = 0;
    i = i | (codes.imm << 12);
    
    if (codes.rd != 0)
        registers[codes.rd] = i;

    *program_count += 4;
}

void xor(LOGIC_OP_ARGS) {

    //printf("xor R[%i] = R[%i] ^ R[%i]\n", codes.rd, codes.rs1, codes.rs2);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] ^ registers[codes.rs2];

    *program_count += 4;
}

void xori(LOGIC_OP_ARGS) {

    //printf("xori R[%i] = R[%i] ^ %i\n", codes.rd, codes.rs1, codes.imm);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] ^ codes.imm;

    *program_count += 4;
}

void or(LOGIC_OP_ARGS) {

    //printf("or R[%i] = R[%i] | R[%i]\n", codes.rd, codes.rs1, codes.rs2);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] | registers[codes.rs2];

    *program_count += 4;
}

void ori(LOGIC_OP_ARGS) {

    //printf("ori R[%i] = R[%i] | %i\n", codes.rd, codes.rs1, codes.imm);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] | codes.imm;

    *program_count += 4;
}

void and(LOGIC_OP_ARGS) {

    //printf("and R[%i] = R[%i] & %i\n", codes.rd, codes.rs1, codes.rs2);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] & registers[codes.rs2];

    *program_count += 4;
}

void andi(LOGIC_OP_ARGS) {

    //printf("andi R[%i] = R[%i] & %i\n", codes.rd, codes.rs1, codes.imm);

    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] & codes.imm;

    *program_count += 4;
}

void sll(LOGIC_OP_ARGS) {

    //printf("sll R[%i] = R[%i] << R[%i]\n", codes.rd, codes.rs1, codes.rs2);
    
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] << registers[codes.rs2]);

    *program_count += 4;
}

void srl(LOGIC_OP_ARGS) {

    //printf("srl R[%i] = R[%i] >> R[%i]\n", codes.rd, codes.rs1, codes.rs2);

    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] >> registers[codes.rs2]);

    *program_count += 4;
}

void sra(LOGIC_OP_ARGS) {

    //printf("sra\n");

    uint32_t val = 0x0;
    int victim = registers[codes.rs1];
    int shift = registers[codes.rs2];

    if (shift > 31)
        shift = 31;

    val = isolate_bits(victim, shift, 31 - shift) | val;
    val = isolate_bits(victim, 0, shift) << shift | val;

    if (codes.rd != 0)
        registers[codes.rd] = (int32_t) val;

    *program_count += 4;
}

/* Memory access operations */

void lb(LOGIC_OP_ARGS) {

    int mem_address = codes.rs1 + codes.imm;
    
    uint8_t value = (uint8_t) read_from_memory(mem_address, ram, 1);
    value = (int32_t) ((value << 24) >> 24);

    if (codes.rd != 0)
        registers[codes.rd] = value;

    //printf("lb R[%i] = %i\n", codes.rd, value);

    *program_count += 4;
}

void lh(LOGIC_OP_ARGS) {
    int mem_address = codes.rs1 + codes.imm;
    
    uint16_t value = (uint16_t) read_from_memory(mem_address, ram, 2);
    value = (int32_t) ((value << 16) >> 16);

    if (codes.rd != 0)
        registers[codes.rd] = value;

    //printf("lh R[%i] = %i\n", codes.rd, value);
    
    *program_count += 4;
}

void lw(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    
    if (codes.rd != 0)
        registers[codes.rd] = (int32_t) read_from_memory(mem_address, ram, 4);

    //printf("lw R[%i] = %i\n", codes.rd, read_from_memory(mem_address, ram, 4));

    *program_count += 4;
}

void lbu(LOGIC_OP_ARGS) {

    int mem_address = registers[codes.rs1] + codes.imm; 

    if (codes.rd != 0)
        registers[codes.rd] = (uint8_t) read_from_memory(mem_address, ram, 1);

    //printf("lbu R[%i] = %i\n", codes.rd, (uint8_t) read_from_memory(mem_address, ram, 1));

    *program_count += 4;
}

void lhu(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 

    if (codes.rd != 0)
        registers[codes.rd] = (uint16_t) read_from_memory(mem_address, ram, 2);

    //printf("lhu R[%i] = %i\n", codes.rd, (uint16_t) read_from_memory(mem_address, ram, 1));

    *program_count += 4;
}

void sb(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    int value = registers[codes.rs2];

    //printf("sb M[%i] = %i\n", mem_address, value);
    
    write_to_memory(mem_address, value, * program_count, registers, ram, 1);

    *program_count += 4;
}

void sh(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    int value = registers[codes.rs2];

    //printf("sh M[%i] = %i\n", mem_address, value);
    
    write_to_memory(mem_address, value, * program_count, registers, ram, 2);

    *program_count += 4;
}

void sw(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    int value = registers[codes.rs2];

    //printf("sw M[%i] = %i\n", mem_address, value);
    
    write_to_memory(mem_address, value, * program_count, registers, ram, 4);

    *program_count += 4;
}

/* Program flow operations */

void slt(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    //printf("slt if (R[%i] < R[%i]) ? 1 : 0\n", codes.rs1, codes.rs2);

    *program_count += 4;
}

void slti(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    //printf("slti\n");

    *program_count += 4;
}

void sltu(LOGIC_OP_ARGS) { // unsigned
    if (codes.rd != 0)
        registers[codes.rd] = ((uint32_t) registers[codes.rs1] < (uint32_t) registers[codes.rs2]) ? 1 : 0;

    //printf("sltu\n");

    *program_count += 4;
}

void sltiu(LOGIC_OP_ARGS) { // unsigned
    if (codes.rd != 0)
        registers[codes.rd] = ((uint32_t) registers[codes.rs1] < (uint32_t) codes.imm) ? 1 : 0;

    //printf("sltiu\n");

    *program_count += 4;
}

void beq(LOGIC_OP_ARGS) {
    //printf("beq if %i == %i, PC += imm\n", registers[codes.rs1], registers[codes.rs2]);
    
    if (registers[codes.rs1] == registers[codes.rs2])
        *program_count += codes.imm;
    else
        *program_count += 4;
}

void bne(LOGIC_OP_ARGS) {

    //printf("bne if %i != %i, PC += imm\n", registers[codes.rs1], registers[codes.rs2]);
    
    if (registers[codes.rs1] != registers[codes.rs2])
        *program_count += codes.imm;
    else 
        *program_count += 4;
}

void blt(LOGIC_OP_ARGS) {

    //printf("blt if %i < %i, PC += imm\n", registers[codes.rs1], registers[codes.rs2]);
    
    if (registers[codes.rs1] < registers[codes.rs2])
        *program_count += codes.imm;
    else 
        *program_count += 4;
}

void bltu(LOGIC_OP_ARGS) { // unsigned

    //printf("bltu\n");
    
    uint32_t rs1_u = (uint32_t) registers[codes.rs1];
    uint32_t rs2_u = (uint32_t) registers[codes.rs2];

    if (rs1_u < rs2_u) {
        *program_count += (uint32_t) codes.imm;
    } else 
        *program_count += 4;
}

void bge(LOGIC_OP_ARGS) {

    //printf("bge if %i >= %i, PC += imm\n", registers[codes.rs1], registers[codes.rs2]);
    
    if (registers[codes.rs1] >= registers[codes.rs2])
        *program_count += codes.imm;
    else 
        *program_count += 4;
}

void bgeu(LOGIC_OP_ARGS) { // unsigned
    uint32_t rs1_u = (uint32_t) registers[codes.rs1];
    uint32_t rs2_u = (uint32_t) registers[codes.rs2];

    //printf("bgeu\n");

    if (rs1_u >= rs2_u) {
        *program_count += codes.imm;
    } else
        *program_count += 4;


}

void jal(LOGIC_OP_ARGS) {

    //printf("jal R[%i] = PC + 4, PC += imm\n", codes.rd);

    if (codes.rd != 0)
        registers[codes.rd] = *program_count + 4;
    
    *program_count += codes.imm;
}

void jalr(LOGIC_OP_ARGS) {

    //printf("jalr R[%i] = PC+4, PC = %i + %i\n", codes.rd, registers[codes.rs1], codes.imm);

    if (codes.rd != 0)
        registers[codes.rd] = *program_count + 4;

    *program_count = (registers[codes.rs1]) + codes.imm;
}
