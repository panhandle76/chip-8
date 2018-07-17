#include <stdio.h>
#include "cpu-6502.h"

#define ASSERT(cond, action) if(!(cond)) {\
                                 printf("%s:%d ASSERT Failed (%s)\n", __func__, __LINE__, #cond); \
                                 action; \
                             }


bool test_addrmode_immediate()
{
	// testing immediate: this value should load the immediate value (1 byte) from 
	// memory into temporary reg.TL, clear out regs.TH and increment PC.

	// 1) set memory
	set_memory(0x6000, 0xDE);
	// 2) set PC
	regs.pc = 0x6000;
	// invoke handler
	addrmode_immediate();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6001, return false);
	// 3) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0xDE, return false);
	ASSERT(regs.th == 0x0, return false);

	return true;
}

bool test_addrmode_zero_page()
{
	// this mode is only capable of addressing the first 256 bytes, so it will only read 1 byte

	// 1) set memory
	set_memory(0x6000, 0xBB);
	// 2) set PC
	regs.pc = 0x6000;
	// invoke handler
	addrmode_zero_page();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6001, return false);
	// 3) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0xBB, return false);
	ASSERT(regs.th == 0x0, return false);

	return true;
}

bool test_addrmode_zero_page_x()
{
	// this mode is only capable of addressing the first 256 bytes, so it will only read 1 byte
	// and will add the X register to it

	// 1) set memory
	set_memory(0x6000, 0xCC);
	// 2) set PC
	regs.pc = 0x6000;
	// 3) set X register
	regs.x = 0x44;
	// invoke handler
	addrmode_zero_page_x();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6001, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0x10, return false);
	ASSERT(regs.th == 0x0, return false);

	return true;
}

bool test_addrmode_zero_page_y()
{
	// this mode is only capable of addressing the first 256 bytes, so it will only read 1 byte
	// and will add the Y register to it

	// 1) set memory
	set_memory(0x6000, 0xDD);
	// 2) set PC
	regs.pc = 0x6000;
	// 3) set X register
	regs.y = 0x44;
	// invoke handler
	addrmode_zero_page_y();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6001, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0x21, return false);
	ASSERT(regs.th == 0x0, return false);

	return true;
}

bool test_addrmode_abs()
{
	// this mode is only capable of addressing all memory, so it will read 2 bytes.

	// 1) set memory
	set_memory(0x6000, 0xAD);
	set_memory(0x6001, 0xDE);
	// 2) set PC
	regs.pc = 0x6000;
	// invoke handler
	addrmode_abs();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6002, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0xAD, return false);
	ASSERT(regs.th == 0xDE, return false);

	return true;
}

bool test_addrmode_abs_x()
{
	// this mode is only capable of addressing all memory, so it will read 2 bytes, then add register X to it.

	// 1) set memory
	set_memory(0x6000, 0xEF);
	set_memory(0x6001, 0xBE);
	// 2) set PC
	regs.pc = 0x6000;
	// 3) set X register
	regs.x = 0x55;
	// invoke handler
	addrmode_abs_x();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6002, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0x44, return false);
	ASSERT(regs.th == 0xBF, return false);

	return true;
}

bool test_addrmode_abs_y()
{
	// this mode is only capable of addressing all memory, so it will read 2 bytes, then add register Y to it.

	// 1) set memory
	set_memory(0x6000, 0xEF);
	set_memory(0x6001, 0xBE);
	// 2) set PC
	regs.pc = 0x6000;
	// 3) set X register
	regs.y = 0x44;
	// invoke handler
	addrmode_abs_y();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6002, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0x33, return false);
	ASSERT(regs.th == 0xBF, return false);

	return true;
}

bool test_addrmode_indirect()
{
	// 1) set memory
	set_memory(0x6000, 0xFE);
	set_memory(0x6001, 0xCA);
	set_memory(0xCAFE, 0xAB);
	set_memory(0xCAFF, 0xCD);
	// 2) set PC
	regs.pc = 0x6000;
	// invoke handler
	addrmode_indirect();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6002, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0xAB, return false);
	ASSERT(regs.th == 0xCD, return false);

	return true;
}

bool test_addrmode_indexed_indirect_x()
{
	// 1) set memory
	set_memory(0x6000, 0x55);
	set_memory(0x78, 0x22);
	set_memory(0x79, 0x33);
	// 2) set PC
	regs.pc = 0x6000;
	//
	regs.x = 0x23;
	// invoke handler
	addrmode_indexed_indirect_x();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6001, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0x22, return false);
	ASSERT(regs.th == 0x33, return false);

	return true;
}


bool test_addrmode_indirect_indexed_y()
{
	// 1) set memory
	set_memory(0x6000, 0x66);
	set_memory(0x66, 0x55);
	set_memory(0x67, 0x66);
	// 2) set PC
	regs.pc = 0x6000;
	//
	regs.y = 0x32;
	// invoke handler
	addrmode_indirect_indexed_y();
	// 4) verify PC incremented
	ASSERT(regs.pc == 0x6001, return false);
	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
	ASSERT(regs.tl == 0x87, return false);
	ASSERT(regs.th == 0x66, return false);

	return true;
}


bool test_c_opcodes()
{
	// clear carry
	regs.status.carry = 1;
	opcode_clc();
	ASSERT(regs.status.carry == 0, return false);

	// clear decimal mode
	regs.status.decimal = 1;
	opcode_cld();
	ASSERT(regs.status.decimal == 0, return false);

	// clear interrupt
	regs.status.interrupt = 1;
	opcode_cli();
	ASSERT(regs.status.interrupt == 0, return false);

	// clear overflow
	regs.status.overflow = 1;
	opcode_clv();
	ASSERT(regs.status.overflow == 0, return false);

	// test compare ACC >=
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x55;
	regs.tl = 0x55;
	//
	opcode_cmp();
	ASSERT(regs.status.carry == 1, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// test compare ACC <
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x19;
	regs.tl = 0x20;
	//
	opcode_cmp();
	ASSERT(regs.status.carry == 0, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// test compare ACC >
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x20;
	regs.tl = 0x19;
	//
	opcode_cmp();
	ASSERT(regs.status.carry == 1, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// test compare X >=
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.x = 0x55;
	regs.tl = 0x55;
	//
	opcode_cpx();
	ASSERT(regs.status.carry == 1, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// test compare X <
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.x = 0x19;
	regs.tl = 0x20;
	//
	opcode_cpx();
	ASSERT(regs.status.carry == 0, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// test compare X >
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.x = 0x20;
	regs.tl = 0x19;
	//
	opcode_cpx();
	ASSERT(regs.status.carry == 1, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// test compare Y >=
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.y = 0x55;
	regs.tl = 0x55;
	//
	opcode_cpy();
	ASSERT(regs.status.carry == 1, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// test compare Y <
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.y = 0x19;
	regs.tl = 0x20;
	//
	opcode_cpy();
	ASSERT(regs.status.carry == 0, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// test compare Y >
	regs.status.carry = 0;
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.y = 0x20;
	regs.tl = 0x19;
	//
	opcode_cpy();
	ASSERT(regs.status.carry == 1, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	return true;
}


bool test_d_opcodes()
{
	// test decrement Memory
	set_memory(0x1234, 0x55);
	regs.tl = 0x34;
	regs.th = 0x12;
	opcode_dec();
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	set_memory(0x1234, 0x01);
	regs.tl = 0x34;
	regs.th = 0x12;
	opcode_dec();
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	set_memory(0x1234, 0x00);
	regs.tl = 0x34;
	regs.th = 0x12;
	opcode_dec();
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// test decrement X
	regs.x = 0x55;
	opcode_dex();
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.x = 0x01;
	opcode_dex();
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.x = 0x00;
	opcode_dex();
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// test decrement Y
	regs.y = 0x55;
	opcode_dey();
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.y = 0x01;
	opcode_dey();
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.y = 0x00;
	opcode_dey();
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	return true;
}


bool test_s_opcodes()
{
	regs.status.carry = 0;
	opcode_sec();
	ASSERT(regs.status.carry == 1, return false);

	regs.status.decimal = 0;
	opcode_sed();
	ASSERT(regs.status.decimal == 1, return false);

	regs.status.interrupt = 0;
	opcode_sei();

	// set temp. and A registers
	regs.tl = 0x34;
	regs.th = 0x12;
	regs.acc = 0xBB;
	//
	opcode_sta();
	//
	ASSERT(get_memory(0x1234) == 0xBB, return false);

	// set temp. and A registers
	regs.tl = 0x34;
	regs.th = 0x12;
	regs.x = 0xCC;
	//
	opcode_stx();
	//
	ASSERT(get_memory(0x1234) == 0xCC, return false);

	// set temp. and A registers
	regs.tl = 0x34;
	regs.th = 0x12;
	regs.y = 0xDD;
	//
	opcode_sty();
	//
	ASSERT(get_memory(0x1234) == 0xDD, return false);

	return true;
}


int main(int argc, char** argv)
{
	bool rtn = test_addrmode_immediate();
	rtn &= test_addrmode_zero_page();
	rtn &= test_addrmode_zero_page_x();
	rtn &= test_addrmode_zero_page_y();
	rtn &= test_addrmode_abs();
	rtn &= test_addrmode_abs_x();
	rtn &= test_addrmode_abs_y();
	rtn &= test_addrmode_indirect();
	rtn &= test_addrmode_indexed_indirect_x();
	rtn &= test_addrmode_indirect_indexed_y();

	// rtn &= test_a_opcodes();
	// rtn &= test_b_opcodes();
	rtn &= test_c_opcodes();
	rtn &= test_d_opcodes();
	// rtn &= test_ei_opcodes();
	// rtn &= test_j_opcodes();
	// rtn &= test_l_opcodes();
	// rtn &= test_no_opcodes();
	// rtn &= test_p_opcodes();
	// rtn &= test_r_opcodes();
	rtn &= test_s_opcodes();
	// rtn &= test_t_opcodes();

	printf("test result: %s\n", (rtn) ? "SUCCESS" : "FAILURE");
	return rtn;
}
