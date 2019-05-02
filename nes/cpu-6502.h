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
  /// registers to temporarily hold addresses. holds address during decode step for all non immediate address mode
  uint8_t  tl;
  uint8_t  th;
  // input data latch register
  uint8_t idl;
};

// locals
struct proc_regs_t  regs;
uint8_t             mem_map[0x10000];

uint8_t get_proc_status();

void set_proc_status(uint8_t status);

void set_memory(uint16_t addr, uint8_t value);
void set_memory_range(uint16_t addr, uint8_t *buffer, uint8_t size);

uint8_t get_memory(uint16_t addr);

void cpu_step();

void cpu_init();

void cpu_load_prg(const uint8_t* prg, uint8_t numblk);

void cpu_reset();
