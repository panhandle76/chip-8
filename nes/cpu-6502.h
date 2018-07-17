#include <stdint.h> //uint8_t

#pragma once

//
typedef uint8_t bool;
#define true 1
#define false 0

// processor status flags
struct proc_status_t
{
  uint8_t  carry;
  uint8_t  zero;
  uint8_t  interrupt;
  uint8_t  decimal;
  uint8_t  break_cmd;
  uint8_t  overflow;
  uint8_t  negative;
};

// program accessable registers
struct proc_regs_t
{
  uint16_t pc; // program counter
  uint8_t  sp;
  uint8_t  acc;
  uint8_t  x;
  uint8_t  y;
  struct proc_status_t status;
  uint8_t  ir; // instruction register
  uint8_t  tl; // temp. register (lower half). holds operand during decode step
  uint8_t  th; // temp. register (upper half). holds operand during decode step
};

// locals
struct proc_regs_t  regs;
uint8_t             mem_map[0x10000];

uint8_t get_proc_status();

void set_proc_status(uint8_t status);

void set_memory(uint16_t addr, uint8_t value);

uint8_t get_memory(uint16_t addr);

void cpu_run();

void cpu_init();

void cpu_load_prg(const uint8_t* prg, uint8_t numblk);

void cpu_reset();

//////////////////////////

void addrmode_immediate();

void addrmode_zero_page();

void addrmode_zero_page_x();

void addrmode_zero_page_y();

void addrmode_abs();

void addrmode_abs_x();

void addrmode_abs_y();

void addrmode_indirect();

void addrmode_indexed_indirect_x();

void addrmode_indirect_indexed_y();

//////////////////////////

void opcode_clc();
void opcode_cld();
void opcode_cli();
void opcode_clv();

void opcode_cmp();
void opcode_cpx();
void opcode_cpy();

void opcode_dec();
void opcode_dex();
void opcode_dey();

void opcode_sec();
void opcode_sed();
void opcode_sei();

void opcode_sta();
void opcode_stx();
void opcode_sty();
