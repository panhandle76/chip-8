#include "cpu-6502.h"
#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>

#define MEM_16KB (16*1024)

//
char* opcode_name[256];
uint8_t opcode_cycles[256];
uint8_t opcode_len[256];
void (*decode_handler[256]) ();
void (*exec_handler[256]) ();


uint8_t get_proc_status()
{
  return (regs.status.negative << 7) | 
         (regs.status.overflow << 6) | 
         (regs.status.break_cmd << 4) | 
         (regs.status.decimal << 3) | 
         (regs.status.interrupt << 2) | 
         (regs.status.zero << 1) | 
          regs.status.carry;
}

void set_proc_status(uint8_t status)
{
  // MSB           LSB
  // [7 6 5 4 3 2 1 0]
  // [N V - B D I Z C]
  regs.status.carry     =  status       & 0x01;
  regs.status.zero      = (status >> 1) & 0x01;
  regs.status.interrupt = (status >> 2) & 0x01;
  regs.status.decimal   = (status >> 3) & 0x01;
  regs.status.break_cmd = (status >> 4) & 0x01;
  regs.status.overflow  = (status >> 6) & 0x01;
  regs.status.negative  = (status >> 7) & 0x01;
}


/**
 for debug/test purposes
 */
void set_memory(uint16_t addr, uint8_t value)
{
  mem_map[addr] = value;
}


void set_memory_range(uint16_t addr, uint8_t *buffer, uint8_t size)
{
  for (int i=0; i<size; i++)
    mem_map[addr+i] = buffer[i];
}


uint8_t get_memory(uint16_t addr)
{
  return mem_map[addr];
}


/**
 */
void print_regs()
{
  // a     x     y
  // pc    sp    status[N V - B D I Z C]
  printf("acc:0x%x\tx:0x%x\ty:0x%x\n", regs.acc, regs.x, regs.y);
  printf("pc:0x%x\tsp:0x%x\t[%s %s - %s %s %s %s %s]\n", regs.pc, regs.sp, (regs.status.negative)?"1":"0", (regs.status.overflow)?"1":"0", (regs.status.break_cmd)?"1":"0", (regs.status.decimal)?"1":"0", (regs.status.interrupt)?"1":"0", (regs.status.zero)?"1":"0", (regs.status.carry)?"1":"0");
  printf("\t\t\t[N O - B D I Z C]\n\n");
}

void cpu_step()
{
  // fetch - next opcode from memory
    regs.ir = mem_map[regs.pc];
    print_regs();
    regs.pc++;

    if (opcode_len[regs.ir] == 0)
    {
      printf("unknown opcode 0x%x\n", regs.ir);
      return;
    }

    // decode - see what else the opcode needs
    if (decode_handler[regs.ir])
    {
      decode_handler[regs.ir]();
    }

    // execute
    if (exec_handler[regs.ir])
    {
      exec_handler[regs.ir]();
    }
}

void cpu_run()
{
  while (1)
  {
    cpu_step();

    // if break command is set then stop
    if (regs.status.break_cmd)
      return;

    // store
  }
}


//////////////////////
// address handlers //
//////////////////////


/**
 These instructions have their data defined as the next byte after the opcode. ORA #$B2 will perform a logical 
 (also called bitwise) of the value B2 with the accumulator. Remember that in assembly when you see a # sign, 
 it indicates an immediate value. If $B2 was written without a #, it would indicate an address or offset.
 */
void addrmode_immediate()
{
  printf("%s: 0x%x\n", __func__, regs.pc);
  regs.tl = mem_map[regs.pc++];
  regs.th = 0;
}


/**
 Zero-Page is an addressing mode that is only capable of addressing the first 256 bytes of the CPU's memory map. 
 You can think of it as absolute addressing for the first 256 bytes. The instruction LDA $35 will put the value 
 stored in memory location $35 into A. The advantage of zero-page are two - the instruction takes one less byte 
 to specify, and it executes in less CPU cycles. Most programs are written to store the most frequently used 
 variables in the first 256 memory locations so they can take advantage of zero page addressing.
 */
void addrmode_zero_page()
{
  printf("%s: pc:0x%x\n", __func__, regs.pc);
  regs.tl = mem_map[regs.pc++];
  regs.th = 0;
}


/**
 This works just like absolute indexed, but the target address is limited to the first 0xFF bytes.

 The target address will wrap around and will always be in the zero page. If the instruction is LDA $C0,X, 
 and X is $60, then the target address will be $20. $C0+$60 = $120, but the carry is discarded in the 
 calculation of the target address.
 */
void addrmode_zero_page_x()
{
  printf("%s: pc:0x%x\n", __func__, regs.pc);
  regs.tl = (mem_map[regs.pc] + regs.x) & 0xFF;
  regs.th = 0;
  regs.pc++;
}


void addrmode_zero_page_y()
{
  printf("%s: pc:0x%x\n", __func__, regs.pc);
  regs.tl = (mem_map[regs.pc] + regs.y) & 0xFF;
  regs.th = 0;
  regs.pc++;
}


/**
 Absolute addressing specifies the memory location explicitly in the two bytes following the opcode. 
 So JMP $4032 will set the PC to $4032. The hex for this is 4C 32 40. The 6502 is a little endian machine, 
 so any 16 bit (2 byte) value is stored with the LSB first. All instructions that use absolute addressing are 3 bytes.
 */
void addrmode_abs()
{
  printf("%s: 0x%x\n", __func__, regs.pc);
  // get the next two bytes after opcode. this is the address to read
  regs.tl = mem_map[regs.pc++];
  regs.th = mem_map[regs.pc++];
}


/**
 This addressing mode makes the target address by adding the contents of the X or Y register to an absolute address. 
 For example, this 6502 code can be used to fill 10 bytes with $FF starting at address $1009, counting down to address $1000.
   LDA #$FF
   LDY #$09
   loop:
   STA $1000,Y ; absolute indexed addressing
   DEY
   BPL loop
 */
void addrmode_abs_x()
{
  printf("%s: 0x%x\n", __func__, regs.pc);
  uint16_t tmp = mem_map[regs.pc++];
           tmp |= (mem_map[regs.pc++] << 8);
           tmp += regs.x;

  regs.tl = tmp & 0xFF;
  regs.th = (tmp >> 8) & 0xFF;
}


void addrmode_abs_y()
{
  printf("%s: 0x%x\n", __func__, regs.pc);
  uint16_t tmp = mem_map[regs.pc++];
           tmp |= (mem_map[regs.pc++] << 8);
           tmp += regs.y;

  regs.tl = tmp & 0xFF;
  regs.th = (tmp >> 8) & 0xFF;
}


/**
 The JMP instruction is the only instruction that uses this addressing mode. It is a 3 byte instruction - the 2nd 
 and 3rd bytes are an absolute address. The set the PC to the address stored at that address. So maybe this would be clearer.

   Memory:
   1000 52 3a 04 d3 93 00 3f 93 84

   Instruction:
   JMP  ($1000)

   When this instruction is executed, the PC will be set to $3a52, which is the address stored at address $1000.
 */
void addrmode_indirect()
{
  printf("%s: 0x%x\n", __func__, regs.pc);
  uint16_t tmp = mem_map[regs.pc++];
           tmp |= mem_map[regs.pc++] << 8;

  regs.tl = mem_map[tmp];
  regs.th = mem_map[tmp+1];
}


/**
  This mode is only used with the X register. Consider a situation where the instruction is LDA ($20,X), 
  X contains $04, and memory at $24 contains 0024: 74 20, First, X is added to $20 to get $24. The target 
  address will be fetched from $24 resulting in a target address of $2074. Register A will be loaded with 
  the contents of memory at $2074.

  If X + the immediate byte will wrap around to a zero-page address. So you could code that like 
  targetAddress = (X + opcode[1]) & 0xFF .

  Indexed Indirect instructions are 2 bytes - the second byte is the zero-page address - $20 in the example. 
  Obviously the fetched address has to be stored in the zero page.
 */
void addrmode_indexed_indirect_x()
{
  uint8_t zpage = (mem_map[regs.pc++] + regs.x) & 0xFF;
  printf("%s: 0x%x\n", __func__, zpage);

  regs.tl = mem_map[zpage];
  regs.th = mem_map[zpage+1];
}


/**
 This mode is only used with the Y register. It differs in the order that Y is applied to the indirectly 
 fetched address. An example instruction that uses indirect index addressing is LDA ($86),Y . To calculate 
 the target address, the CPU will first fetch the address stored at zero page location $86. That address 
 will be added to register Y to get the final target address. For LDA ($86),Y, if the address stored 
 at $86 is $4028 (memory is 0086: 28 40, remember little endian) and register Y contains $10, then the final 
 target address would be $4038. Register A will be loaded with the contents of memory at $4038.

 Indirect Indexed instructions are 2 bytes - the second byte is the zero-page address - $20 in the example. 
 (So the fetched address has to be stored in the zero page.)

 While indexed indirect addressing will only generate a zero-page address, this mode's target address is 
 not wrapped - it can be anywhere in the 16-bit address space.
 */
void addrmode_indirect_indexed_y()
{
  uint8_t zpage = mem_map[regs.pc++];
  uint16_t tmp = (mem_map[zpage] | (mem_map[zpage+1] << 8)) + regs.y;
  printf("%s: 0x%x\n", __func__, tmp);

  regs.tl = tmp & 0xFF;
  regs.th = (tmp >> 8) & 0xFF;
}


/////////////
// opcodes //
/////////////


void opcode_adc()
{
  // adjust the ACC register and set carry flag
  if ((regs.acc + regs.status.carry + regs.tl) > 0xff)
  {
    regs.acc += regs.status.carry + regs.tl;
    regs.status.carry = 1;
  }
  else
  {
    regs.acc += regs.status.carry + regs.tl;
    regs.status.carry = 0;
  }

  // TODO : finish

  // set zero flag
  regs.status.zero = (regs.acc==0) ? 1 : 0;

  // set overflow

  // set negative
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_and()
{
  regs.acc &= regs.tl;

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_asl()
{// TODO : finish
}


void opcode_bcc()
{// TODO : finish
}


void opcode_bcs()
{// TODO : finish
}


void opcode_beq()
{// TODO : finish
}


void opcode_bit()
{
  uint8_t test = mem_map[regs.tl | (regs.th << 8)] & regs.acc;

  // set flags
  regs.status.zero = (test==0) ? 1 : 0;
  regs.status.overflow = (test & 0x40) ? 1 : 0;
  regs.status.negative = (test & 0x80) ? 1 : 0;
}


void opcode_bmi()
{// TODO : finish
}


void opcode_bne()
{// TODO : finish
}


void opcode_bpl()
{// TODO : finish
}


void opcode_brk()
{// TODO : finish
  regs.status.break_cmd = 1;
}


void opcode_bvc()
{// TODO : finish
}


void opcode_bvs()
{// TODO : finish
}


void opcode_clc()
{
  regs.status.carry = 0;
}


void opcode_cld()
{
  regs.status.decimal = 0;
}


void opcode_cli()
{
  regs.status.interrupt = 0;
}


void opcode_clv()
{
  regs.status.overflow = 0;
}


void opcode_cmp()
{
  // assumes the value from memory to compare is loaded into regs.tl
  int8_t result;
  if (regs.ir == 0xc9)
    result = regs.acc - regs.tl;
  else
    result = regs.acc - mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.carry = (result >= 0) ? 1 : 0;
  regs.status.zero = (result == 0) ? 1 : 0;
  regs.status.negative = (result & 0x80) ? 1 : 0;
}


void opcode_cpx()
{
  int8_t result;
  if (regs.ir == 0xe0)
    result = regs.x - regs.tl;
  else
    result = regs.x - mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.carry = (result >= 0) ? 1 : 0;
  regs.status.zero = (result == 0) ? 1 : 0;
  regs.status.negative = (result & 0x80) ? 1 : 0;
}


void opcode_cpy()
{
  int8_t result;
  if (regs.ir == 0xc0)
    result = regs.y - regs.tl;
  else
    result = regs.y - mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.carry = (result >= 0) ? 1 : 0;
  regs.status.zero = (result == 0) ? 1 : 0;
  regs.status.negative = (result & 0x80) ? 1 : 0;
}


void opcode_dec()
{
  uint8_t result = mem_map[regs.tl | (regs.th << 8)] - 1;
  mem_map[regs.tl | (regs.th << 8)] = result;

  // set flags
  regs.status.zero = (result==0) ? 1 : 0;
  regs.status.negative = (result & 0x80) ? 1 : 0;
}


void opcode_dex()
{
  regs.x--;

  // set flags
  regs.status.zero = (regs.x==0) ? 1 : 0;
  regs.status.negative = (regs.x & 0x80) ? 1 : 0;
}


void opcode_dey()
{
  regs.y--;

  // set flags
  regs.status.zero = (regs.y==0) ? 1 : 0;
  regs.status.negative = (regs.y & 0x80) ? 1 : 0;
}


void opcode_eor()
{
  // if immediate value use it; otherwise read memory???? TODO THIS MAY BE BAD TO DO
  if (regs.ir == 0x49)
    regs.acc ^= regs.tl;
  else
    regs.acc ^= mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_inc()
{
  uint8_t result = mem_map[regs.tl | (regs.th << 8)] + 1;
  mem_map[regs.tl | (regs.th << 8)] = result;

  // set flags
  regs.status.zero = (result==0) ? 1 : 0;
  regs.status.negative = (result & 0x80) ? 1 : 0;
}


void opcode_inx()
{
  regs.x++;

  // set flags
  regs.status.zero = (regs.x==0) ? 1 : 0;
  regs.status.negative = (regs.x & 0x80) ? 1 : 0;
}


void opcode_iny()
{
  regs.y++;

  // set flags
  regs.status.zero = (regs.y==0) ? 1 : 0;
  regs.status.negative = (regs.y & 0x80) ? 1 : 0;
}


void opcode_jmp()
{
  // TODO : finish
  regs.pc = (regs.th << 8) | regs.tl;
}


void opcode_jsr()
{// TODO : finish

  // push address onto stack
  //

  // set program counter
  //
}


void opcode_lda()
{
  if (regs.ir == 0xa9)
    regs.acc = regs.tl;
  else
    regs.acc = mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_ldx()
{
  if (regs.ir == 0xa2)
    regs.x = regs.tl;
  else
    regs.x = mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.zero = (regs.x==0) ? 1 : 0;
  regs.status.negative = (regs.x & 0x80) ? 1 : 0;
}


void opcode_ldy()
{
  if (regs.ir == 0xa0)
    regs.y = regs.tl;
  else
    regs.y = mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.zero = (regs.y==0) ? 1 : 0;
  regs.status.negative = (regs.y & 0x80) ? 1 : 0;
}


void opcode_lsr()
{
  if (regs.ir == 0x4a)
  {
    regs.status.carry = regs.acc & 0x1;
    regs.acc = ((regs.acc >> 1) & 0x7f);

    // set flags
    regs.status.zero = (regs.acc == 0) ? 1 : 0;
    regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
  }
  else
  {
    uint16_t addr = regs.tl | (regs.th << 8);
    regs.status.carry = mem_map[addr] & 0x1;
    mem_map[addr] = (mem_map[addr] >> 1) & 0x7f;

    // set flags
    regs.status.zero = (mem_map[addr] == 0) ? 1 : 0;
    regs.status.negative = (mem_map[addr] & 0x80) ? 1 : 0;
  }
}


void opcode_nop()
{
}


void opcode_ora()
{
  if (regs.ir == 0x09)
    regs.acc |= regs.tl;
  else
    regs.acc |= mem_map[regs.tl | (regs.th << 8)];

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_pha()
{
  // push onto stack; decrement
  mem_map[regs.sp] = regs.acc;
  regs.sp--;
}


void opcode_php()
{
  // push onto stack; decrement
  mem_map[regs.sp] = get_proc_status();
  regs.sp--;
}


void opcode_pla()
{
  // increment sp; then grab value
  regs.sp++;
  regs.acc = mem_map[regs.sp];

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_plp()
{
  // increment sp; then grab value
  regs.sp++;
  set_proc_status(mem_map[regs.sp]);
}


// void opcode_rol(uint8_t op, uint8_t v1, uint8_t v2)
// {// TODO : finish
// }


// void opcode_ror(uint8_t op, uint8_t v1, uint8_t v2)
// {// TODO : finish
// }


// void opcode_rti(uint8_t op, uint8_t v1, uint8_t v2)
// {// TODO : finish
// }


// void opcode_rts(uint8_t op, uint8_t v1, uint8_t v2)
// {// TODO : finish
// }


void opcode_sbc()
{// TODO finish
  uint8_t m = mem_map[(regs.th << 8) | regs.tl];
  int16_t result = regs.acc - m - (1-regs.status.carry);

  // set flags
  regs.status.carry = (result & 0x100) ? 1 : 0; // set flag if the value > 0xff
  regs.status.zero = (regs.x==0) ? 1 : 0;
  regs.status.overflow = 0;
  regs.status.negative = (regs.x & 0x80) ? 1 : 0;
}


void opcode_sec()
{
  regs.status.carry = 1;
}


void opcode_sed()
{
  regs.status.decimal = 1;
}


void opcode_sei()
{
  regs.status.interrupt = 1;
}


void opcode_sta()
{
  mem_map[(regs.th << 8) | regs.tl] = regs.acc;
}


void opcode_stx()
{
  mem_map[(regs.th << 8) | regs.tl] = regs.x;
}


void opcode_sty()
{
  mem_map[(regs.th << 8) | regs.tl] = regs.y;
}


void opcode_tax()
{
  regs.x = regs.acc;

  // set flags
  regs.status.zero = (regs.x==0) ? 1 : 0;
  regs.status.negative = (regs.x & 0x80) ? 1 : 0;
}


void opcode_txa()
{
  regs.acc = regs.x;

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_tay()
{
  regs.y = regs.acc;

  // set flags
  regs.status.zero = (regs.y==0) ? 1 : 0;
  regs.status.negative = (regs.y & 0x80) ? 1 : 0;
}


void opcode_tya()
{
  regs.acc = regs.y;

  // set flags
  regs.status.zero = (regs.acc==0) ? 1 : 0;
  regs.status.negative = (regs.acc & 0x80) ? 1 : 0;
}


void opcode_tsx()
{
  regs.x = regs.sp;

  // set flags
  regs.status.zero = (regs.x==0) ? 1 : 0;
  regs.status.negative = (regs.x & 0x80) ? 1 : 0;
}


void opcode_txs()
{ // S = X
  regs.sp = regs.x;
}


#define OPCODE(op, name, len, cycles, exec, decode) opcode_name[op] = name; \
                                                    opcode_len[op] = len; \
                                                    opcode_cycles[op] = cycles; \
                                                    exec_handler[op] = exec; \
                                                    decode_handler[op] = decode;

void cpu_init()
{
  // zero out stuff
  memset(opcode_name, 0, 256);
  memset(opcode_cycles, 0, 256);
  memset(opcode_len, 0, 256);
  memset(decode_handler, 0, sizeof(void*) * 256);
  memset(exec_handler, 0, sizeof(void*) * 256);

  OPCODE(0x69, "adc", 2, 2, opcode_adc, addrmode_immediate)
  OPCODE(0x65, "adc", 2, 3, opcode_adc, addrmode_zero_page)
  OPCODE(0x75, "adc", 2, 4, opcode_adc, addrmode_zero_page_x)
  OPCODE(0x6D, "adc", 3, 4, opcode_adc, addrmode_abs)
  OPCODE(0x7D, "adc", 3, 4, opcode_adc, addrmode_abs_x)
  OPCODE(0x79, "adc", 3, 4, opcode_adc, addrmode_abs_y)
  OPCODE(0x61, "adc", 2, 6, opcode_adc, addrmode_indexed_indirect_x)
  OPCODE(0x71, "adc", 2, 5, opcode_adc, addrmode_indirect_indexed_y)

  OPCODE(0x29, "and", 2, 2, opcode_and, addrmode_immediate)
  OPCODE(0x25, "and", 2, 3, opcode_and, addrmode_zero_page)
  OPCODE(0x35, "and", 2, 4, opcode_and, addrmode_zero_page_x)
  OPCODE(0x2d, "and", 3, 4, opcode_and, addrmode_abs)
  OPCODE(0x3d, "and", 3, 4, opcode_and, addrmode_abs_x)
  OPCODE(0x39, "and", 3, 4, opcode_and, addrmode_abs_y)
  OPCODE(0x21, "and", 2, 6, opcode_and, addrmode_indexed_indirect_x)
  OPCODE(0x31, "and", 2, 5, opcode_and, addrmode_indirect_indexed_y)

  // OPCODE(0x0a, "asl", 1, 2, opcode_asl)
  // OPCODE(0x06, "asl", 2, 5, opcode_asl)
  // OPCODE(0x16, "asl", 2, 6, opcode_asl)
  // OPCODE(0x0e, "asl", 3, 6, opcode_asl)
  // OPCODE(0x1e, "asl", 3, 7, opcode_asl)

  // OPCODE(0x90, "bcc", 2, 2, opcode_bcc)

  // OPCODE(0xb0, "bcs", 2, 2, opcode_bcs)

  // OPCODE(0xf0, "beq", 2, 2, opcode_beq)

  OPCODE(0x24, "bit", 2, 3, opcode_bit, addrmode_zero_page)
  OPCODE(0x2c, "bit", 3, 4, opcode_bit, addrmode_abs)

  // OPCODE(0x30, "bmi", 2, 2, opcode_bmi)

  // OPCODE(0xd0, "bne", 2, 2, opcode_bne)

  // OPCODE(0x10, "bpl", 2, 2, opcode_bpl)

  OPCODE(0x00, "brk", 1, 7, opcode_brk, NULL)

  // OPCODE(0x50, "bvc", 2, 2, opcode_bvc)

  // OPCODE(0x70, "bvs", 2, 2, opcode_bvs)

  OPCODE(0x18, "clc", 1, 2, opcode_clc, NULL)

  OPCODE(0xd8, "cld", 1, 2, opcode_cld, NULL)

  OPCODE(0x58, "cli", 1, 2, opcode_cli, NULL)

  OPCODE(0xb8, "clv", 1, 2, opcode_clv, NULL)

  OPCODE(0xc9, "cmp", 2, 2, opcode_cmp, addrmode_immediate)
  OPCODE(0xc5, "cmp", 2, 3, opcode_cmp, addrmode_zero_page)
  OPCODE(0xd5, "cmp", 2, 4, opcode_cmp, addrmode_zero_page_x)
  OPCODE(0xcd, "cmp", 3, 4, opcode_cmp, addrmode_abs)
  OPCODE(0xdd, "cmp", 3, 4, opcode_cmp, addrmode_abs_x)
  OPCODE(0xd9, "cmp", 3, 4, opcode_cmp, addrmode_abs_y)
  OPCODE(0xc1, "cmp", 2, 6, opcode_cmp, addrmode_indexed_indirect_x)
  OPCODE(0xd1, "cmp", 2, 5, opcode_cmp, addrmode_indirect_indexed_y)

  OPCODE(0xe0, "cpx", 2, 2, opcode_cpx, addrmode_immediate)
  OPCODE(0xe4, "cpx", 2, 3, opcode_cpx, addrmode_zero_page)
  OPCODE(0xec, "cpx", 3, 4, opcode_cpx, addrmode_abs)

  OPCODE(0xc0, "cpy", 2, 2, opcode_cpy, addrmode_immediate)
  OPCODE(0xc4, "cpy", 2, 3, opcode_cpy, addrmode_zero_page)
  OPCODE(0xcc, "cpy", 3, 4, opcode_cpy, addrmode_abs)
  
  OPCODE(0xc6, "dec", 2, 5, opcode_dec, addrmode_zero_page)
  OPCODE(0xd6, "dec", 2, 6, opcode_dec, addrmode_zero_page_x)
  OPCODE(0xce, "dec", 3, 6, opcode_dec, addrmode_abs)
  OPCODE(0xde, "dec", 3, 7, opcode_dec, addrmode_abs_x)

  OPCODE(0xca, "dex", 1, 2, opcode_dex, NULL)
  
  OPCODE(0x88, "dey", 1, 2, opcode_dey, NULL)

  OPCODE(0x49, "eor", 2, 2, opcode_eor, addrmode_immediate)
  OPCODE(0x45, "eor", 2, 3, opcode_eor, addrmode_zero_page)
  OPCODE(0x55, "eor", 2, 4, opcode_eor, addrmode_zero_page_x)
  OPCODE(0x4d, "eor", 3, 4, opcode_eor, addrmode_abs)
  OPCODE(0x5d, "eor", 3, 4, opcode_eor, addrmode_abs_x)
  OPCODE(0x59, "eor", 3, 4, opcode_eor, addrmode_abs_y)
  OPCODE(0x41, "eor", 2, 6, opcode_eor, addrmode_indexed_indirect_x)
  OPCODE(0x51, "eor", 2, 5, opcode_eor, addrmode_indirect_indexed_y)

  OPCODE(0xe6, "inc", 2, 5, opcode_inc, addrmode_zero_page)
  OPCODE(0xf6, "inc", 2, 6, opcode_inc, addrmode_zero_page_x)
  OPCODE(0xee, "inc", 3, 6, opcode_inc, addrmode_abs)
  OPCODE(0xfe, "inc", 3, 7, opcode_inc, addrmode_abs_x)

  OPCODE(0xe8, "inx", 1, 2, opcode_inx, NULL)
  OPCODE(0xc8, "iny", 1, 2, opcode_iny, NULL)

  OPCODE(0x4c, "jmp", 3, 3, opcode_jmp, addrmode_abs)
  OPCODE(0x6c, "jmp", 3, 5, opcode_jmp, addrmode_indirect)

  // OPCODE(0x20, "jsr", 3, 6, opcode_jsr)

  OPCODE(0xa9, "lda", 2, 2, opcode_lda, addrmode_immediate)
  OPCODE(0xa5, "lda", 2, 3, opcode_lda, addrmode_zero_page)
  OPCODE(0xb5, "lda", 2, 4, opcode_lda, addrmode_zero_page_x)
  OPCODE(0xad, "lda", 3, 4, opcode_lda, addrmode_abs)
  OPCODE(0xbd, "lda", 3, 4, opcode_lda, addrmode_abs_x)
  OPCODE(0xb9, "lda", 3, 4, opcode_lda, addrmode_abs_y)
  OPCODE(0xa1, "lda", 2, 6, opcode_lda, addrmode_indexed_indirect_x)
  OPCODE(0xb1, "lda", 2, 5, opcode_lda, addrmode_indirect_indexed_y)

  OPCODE(0xa2, "ldx", 2, 2, opcode_ldx, addrmode_immediate)
  OPCODE(0xa6, "ldx", 2, 3, opcode_ldx, addrmode_zero_page)
  OPCODE(0xb6, "ldx", 2, 4, opcode_ldx, addrmode_zero_page_y)
  OPCODE(0xae, "ldx", 3, 4, opcode_ldx, addrmode_abs)
  OPCODE(0xbe, "ldx", 3, 4, opcode_ldx, addrmode_abs_y)

  OPCODE(0xa0, "ldy", 2, 2, opcode_ldy, addrmode_immediate)
  OPCODE(0xa4, "ldy", 2, 3, opcode_ldy, addrmode_zero_page)
  OPCODE(0xb4, "ldy", 2, 4, opcode_ldy, addrmode_zero_page_x)
  OPCODE(0xac, "ldy", 3, 4, opcode_ldy, addrmode_abs)
  OPCODE(0xbc, "ldy", 3, 4, opcode_ldy, addrmode_abs_x)

  OPCODE(0x4a, "lsr", 1, 2, opcode_lsr, NULL)
  OPCODE(0x46, "lsr", 2, 5, opcode_lsr, addrmode_zero_page)
  OPCODE(0x56, "lsr", 2, 6, opcode_lsr, addrmode_zero_page_x)
  OPCODE(0x4e, "lsr", 3, 6, opcode_lsr, addrmode_abs)
  OPCODE(0x5e, "lsr", 3, 7, opcode_lsr, addrmode_abs_x)

  OPCODE(0xea, "nop", 1, 2, opcode_nop, NULL)

  OPCODE(0x09, "ora", 2, 2, opcode_ora, addrmode_immediate)
  OPCODE(0x05, "ora", 2, 3, opcode_ora, addrmode_zero_page)
  OPCODE(0x15, "ora", 2, 4, opcode_ora, addrmode_zero_page_x)
  OPCODE(0x0d, "ora", 3, 4, opcode_ora, addrmode_abs)
  OPCODE(0x1d, "ora", 3, 4, opcode_ora, addrmode_abs_x)
  OPCODE(0x19, "ora", 3, 4, opcode_ora, addrmode_abs_y)
  OPCODE(0x01, "ora", 2, 6, opcode_ora, addrmode_indexed_indirect_x)
  OPCODE(0x11, "ora", 2, 5, opcode_ora, addrmode_indirect_indexed_y)

  OPCODE(0x48, "pha", 1, 3, opcode_pha, NULL)
  OPCODE(0x08, "php", 1, 3, opcode_php, NULL)
  OPCODE(0x68, "pla", 1, 4, opcode_pla, NULL)
  OPCODE(0x28, "plp", 1, 4, opcode_plp, NULL)

  // OPCODE(0x2a, "rol", 1, 2, opcode_rol)
  // OPCODE(0x26, "rol", 2, 5, opcode_rol)
  // OPCODE(0x36, "rol", 2, 6, opcode_rol)
  // OPCODE(0x2e, "rol", 3, 6, opcode_rol)
  // OPCODE(0x3e, "rol", 3, 7, opcode_rol)

  // OPCODE(0x6a, "ror", 1, 2, opcode_ror)
  // OPCODE(0x66, "ror", 2, 5, opcode_ror)
  // OPCODE(0x76, "ror", 2, 6, opcode_ror)
  // OPCODE(0x6e, "ror", 3, 6, opcode_ror)
  // OPCODE(0x7e, "ror", 3, 7, opcode_ror)

  // OPCODE(0x40, "rti", 1, 6, opcode_rti)

  // OPCODE(0x60, "rts", 1, 6, opcode_rts)

  // OPCODE(0xe9, "sbc", 2, 2, opcode_sbc)
  // OPCODE(0xe5, "sbc", 2, 3, opcode_sbc)
  // OPCODE(0xf5, "sbc", 2, 4, opcode_sbc)
  // OPCODE(0xed, "sbc", 3, 4, opcode_sbc)
  // OPCODE(0xfd, "sbc", 3, 4, opcode_sbc)
  // OPCODE(0xf9, "sbc", 3, 4, opcode_sbc)
  // OPCODE(0xe1, "sbc", 2, 6, opcode_sbc)
  // OPCODE(0xf1, "sbc", 2, 5, opcode_sbc)

  OPCODE(0x38, "sec", 1, 2, opcode_sec, NULL)

  OPCODE(0xf8, "sed", 1, 2, opcode_sed, NULL)

  OPCODE(0x78, "sei", 1, 2, opcode_sei, NULL)

  OPCODE(0x85, "sta", 2, 3, opcode_sta, addrmode_zero_page)
  OPCODE(0x95, "sta", 2, 4, opcode_sta, addrmode_zero_page_x)
  OPCODE(0x8d, "sta", 3, 4, opcode_sta, addrmode_abs)
  OPCODE(0x9d, "sta", 3, 5, opcode_sta, addrmode_abs_x)
  OPCODE(0x99, "sta", 3, 5, opcode_sta, addrmode_abs_y)
  OPCODE(0x81, "sta", 2, 6, opcode_sta, addrmode_indexed_indirect_x)
  OPCODE(0x91, "sta", 2, 6, opcode_sta, addrmode_indirect_indexed_y)

  OPCODE(0x86, "stx", 2, 3, opcode_stx, addrmode_zero_page)
  OPCODE(0x96, "stx", 2, 4, opcode_stx, addrmode_zero_page_y)
  OPCODE(0x8e, "stx", 3, 4, opcode_stx, addrmode_abs)

  OPCODE(0x84, "sty", 2, 3, opcode_sty, addrmode_zero_page)
  OPCODE(0x94, "sty", 2, 4, opcode_sty, addrmode_zero_page_x)
  OPCODE(0x8c, "sty", 3, 4, opcode_sty, addrmode_abs)

  OPCODE(0xaa, "tax", 1, 2, opcode_tax, NULL)
  OPCODE(0x8a, "txa", 1, 2, opcode_txa, NULL)

  OPCODE(0xa8, "tay", 1, 2, opcode_tay, NULL)
  OPCODE(0x98, "tya", 1, 2, opcode_tya, NULL)

  OPCODE(0xba, "tsx", 1, 2, opcode_tsx, NULL)
  OPCODE(0x9a, "txs", 1, 2, opcode_txs, NULL)
}


/**
 * load PRG ROM into memory at 0x8000. if there is only one page, then load 
 * at both 0x8000 and 0xc000; otherwise load two pages worth.
 */
void cpu_load_prg(const uint8_t* prg, uint8_t numblk)
{
  if (numblk == 1)
  {
    printf("memcpy 1 block to 0x8000 and 0xC000\n");
    memcpy(&(mem_map[0x8000]), prg, MEM_16KB);
    memcpy(&(mem_map[0xC000]), prg, MEM_16KB);
  }
  else
  {
    printf("memcpy 2 blocks\n");
    memcpy(&mem_map[0x8000], prg, 2*MEM_16KB);
  }
}


/*
 * reset will set pc to 0xfffc and 0xfffd
 */
void cpu_reset()
{
  regs.pc = (mem_map[0xfffd]<<8) | mem_map[0xfffc];
  printf("reset pc=0x%x\n\n", regs.pc);

  // TODO
}
