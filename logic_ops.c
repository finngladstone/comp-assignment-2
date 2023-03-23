#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bit_ops.c"

void memory_address_invalid(int address) {
    printf("Address %i invalid.\n", address);
    exit(1);
}

/* Default arguments*/

#define LOGIC_OP_ARGS struct data codes, int * registers, int * program_count, int * ram

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

    scanf("%d", &c);

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

void write_to_memory(int address, int value, int program_counter, int * registers, int * ram) {
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
            dump_memory_word(value);
            break;
        }

        default:
        {
            if (within_range(1024, 2047, address)) {
                ram[address] = value;
            } else {
                memory_address_invalid(address);
            }

            break;
        }
    }
}

int read_from_memory(int address, int * ram) {
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
            if (within_range(1024, 2047, address)) {
                return ram[address];
            } else {
                memory_address_invalid(address);
            }
            
            exit(1);
        }
    }
}



/* Logic operataions */

void add(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] + registers[codes.rs2];

    *program_count += 4;
}

void addi(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] + codes.imm;

    *program_count += 4;
}

void sub(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] - registers[codes.rs2];

    *program_count += 4;
}

void lui(LOGIC_OP_ARGS) {
    int32_t i = 0;
    i = i | (codes.imm << 12);
    
    if (codes.rd != 0)
        registers[codes.rd] = i;

    *program_count += 4;
}

void xor(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] ^ registers[codes.rs2];

    *program_count += 4;
}

void xori(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] ^ codes.imm;

    *program_count += 4;
}

void or(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] | registers[codes.rs2];

    *program_count += 4;
}

void ori(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] | codes.imm;

    *program_count += 4;
}

void and(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] & registers[codes.rs2];

    *program_count += 4;
}

void andi(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] & codes.imm;

    *program_count += 4;
}

void sll(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] << registers[codes.rs2];

    *program_count += 4;
}

void srl(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] >> registers[codes.rs2];

    *program_count += 4;
}

void sra(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = registers[codes.rs1] >> registers[codes.rs2];

    *program_count += 4;
}

/* Memory access operations */

void lb(LOGIC_OP_ARGS) {
    // sign extend
}

void lh(LOGIC_OP_ARGS) {
    // sign extend
}

void lw(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    
    if (codes.rd != 0)
        registers[codes.rd] = (int32_t) read_from_memory(mem_address, ram);

    *program_count += 4;
}

void lbu(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 

    if (codes.rd != 0)
        registers[codes.rd] = (int8_t) read_from_memory(mem_address, ram);

    *program_count += 4;
}

void lhu(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 

    if (codes.rd != 0)
        registers[codes.rd] = (int16_t) read_from_memory(mem_address, ram);

    *program_count += 4;
}

void sb(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    int value = registers[codes.rs2];
    
    write_to_memory(mem_address, (int8_t) value, * program_count, registers, ram);

    *program_count += 4;
}

void sh(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    int value = registers[codes.rs2];
    
    write_to_memory(mem_address, (int16_t) value, * program_count, registers, ram);

    *program_count += 4;
}

void sw(LOGIC_OP_ARGS) {
    int mem_address = registers[codes.rs1] + codes.imm; 
    int value = registers[codes.rs2];
    
    write_to_memory(mem_address, (int32_t) value, * program_count, registers, ram);

    *program_count += 4;
}

/* Program flow operations */

void slt(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    *program_count += 4;
}

void slti(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    *program_count += 4;
}

void sltu(LOGIC_OP_ARGS) { // unsigned
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] < registers[codes.rs2]) ? 1 : 0;

    *program_count += 4;
}

void sltiu(LOGIC_OP_ARGS) { // unsigned
    if (codes.rd != 0)
        registers[codes.rd] = (registers[codes.rs1] < codes.imm) ? 1 : 0;

    *program_count += 4;
}

void beq(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] == registers[codes.rs2])
        *program_count += (codes.imm << 1);
}

void bne(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] != registers[codes.rs2])
        *program_count += (codes.imm << 1);
}

void blt(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] < registers[codes.rs2])
        *program_count += (codes.imm << 1);
}

void bltu(LOGIC_OP_ARGS) { // unsigned
    
}

void bge(LOGIC_OP_ARGS) {
    
    if (registers[codes.rs1] >= registers[codes.rs2])
        *program_count += (codes.imm << 1);
}

void bgeu(LOGIC_OP_ARGS) {} // unsigned

void jal(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = *program_count + 4;
    
    *program_count += codes.imm;
}

void jalr(LOGIC_OP_ARGS) {
    if (codes.rd != 0)
        registers[codes.rd] = *program_count + 4;

    *program_count = (registers[codes.rs1]) + codes.imm;
}
