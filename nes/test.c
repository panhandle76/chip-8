#include <stdio.h>
#include "cpu-6502.h"

#define ASSERT(cond, action) if(!(cond)) {\
                                 printf("%s:%d ASSERT Failed (%s)\n", __func__, __LINE__, #cond); \
                                 action; \
                             }


// bool test_addrmode_immediate()
// {
// 	// testing immediate: this value should load the immediate value (1 byte) from 
// 	// memory into temporary reg.TL, clear out regs.TH and increment PC.

// 	// 1) set memory
// 	set_memory(0x6000, 0xDE);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// invoke handler
// 	addrmode_immediate();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6001, return false);
// 	// 3) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0xDE, return false);
// 	ASSERT(regs.th == 0x0, return false);

// 	return true;
// }

// bool test_addrmode_zero_page()
// {
// 	// this mode is only capable of addressing the first 256 bytes, so it will only read 1 byte

// 	// 1) set memory
// 	set_memory(0x6000, 0xBB);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// invoke handler
// 	addrmode_zero_page();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6001, return false);
// 	// 3) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0xBB, return false);
// 	ASSERT(regs.th == 0x0, return false);

// 	return true;
// }

// bool test_addrmode_zero_page_x()
// {
// 	// this mode is only capable of addressing the first 256 bytes, so it will only read 1 byte
// 	// and will add the X register to it

// 	// 1) set memory
// 	set_memory(0x6000, 0xCC);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// 3) set X register
// 	regs.x = 0x44;
// 	// invoke handler
// 	addrmode_zero_page_x();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6001, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0x10, return false);
// 	ASSERT(regs.th == 0x0, return false);

// 	return true;
// }

// bool test_addrmode_zero_page_y()
// {
// 	// this mode is only capable of addressing the first 256 bytes, so it will only read 1 byte
// 	// and will add the Y register to it

// 	// 1) set memory
// 	set_memory(0x6000, 0xDD);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// 3) set X register
// 	regs.y = 0x44;
// 	// invoke handler
// 	addrmode_zero_page_y();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6001, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0x21, return false);
// 	ASSERT(regs.th == 0x0, return false);

// 	return true;
// }

// bool test_addrmode_abs()
// {
// 	// this mode is only capable of addressing all memory, so it will read 2 bytes.

// 	// 1) set memory
// 	set_memory(0x6000, 0xAD);
// 	set_memory(0x6001, 0xDE);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// invoke handler
// 	addrmode_abs();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6002, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0xAD, return false);
// 	ASSERT(regs.th == 0xDE, return false);

// 	return true;
// }

// bool test_addrmode_abs_x()
// {
// 	// this mode is only capable of addressing all memory, so it will read 2 bytes, then add register X to it.

// 	// 1) set memory
// 	set_memory(0x6000, 0xEF);
// 	set_memory(0x6001, 0xBE);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// 3) set X register
// 	regs.x = 0x55;
// 	// invoke handler
// 	addrmode_abs_x();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6002, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0x44, return false);
// 	ASSERT(regs.th == 0xBF, return false);

// 	return true;
// }

// bool test_addrmode_abs_y()
// {
// 	// this mode is only capable of addressing all memory, so it will read 2 bytes, then add register Y to it.

// 	// 1) set memory
// 	set_memory(0x6000, 0xEF);
// 	set_memory(0x6001, 0xBE);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// 3) set X register
// 	regs.y = 0x44;
// 	// invoke handler
// 	addrmode_abs_y();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6002, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0x33, return false);
// 	ASSERT(regs.th == 0xBF, return false);

// 	return true;
// }

// bool test_addrmode_indirect()
// {
// 	// 1) set memory
// 	set_memory(0x6000, 0xFE);
// 	set_memory(0x6001, 0xCA);
// 	set_memory(0xCAFE, 0xAB);
// 	set_memory(0xCAFF, 0xCD);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	// invoke handler
// 	addrmode_indirect();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6002, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0xAB, return false);
// 	ASSERT(regs.th == 0xCD, return false);

// 	return true;
// }

// bool test_addrmode_indexed_indirect_x()
// {
// 	// 1) set memory
// 	set_memory(0x6000, 0x55);
// 	set_memory(0x78, 0x22);
// 	set_memory(0x79, 0x33);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	//
// 	regs.x = 0x23;
// 	// invoke handler
// 	addrmode_indexed_indirect_x();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6001, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0x22, return false);
// 	ASSERT(regs.th == 0x33, return false);

// 	return true;
// }


// bool test_addrmode_indirect_indexed_y()
// {
// 	// 1) set memory
// 	set_memory(0x6000, 0x66);
// 	set_memory(0x66, 0x55);
// 	set_memory(0x67, 0x66);
// 	// 2) set PC
// 	regs.pc = 0x6000;
// 	//
// 	regs.y = 0x32;
// 	// invoke handler
// 	addrmode_indirect_indexed_y();
// 	// 4) verify PC incremented
// 	ASSERT(regs.pc == 0x6001, return false);
// 	// 5) verifing the regs.TL  (temp. lower) and regs.TH (temp. high) registers have the correct value
// 	ASSERT(regs.tl == 0x87, return false);
// 	ASSERT(regs.th == 0x66, return false);

// 	return true;
// }


// bool test_a_opcodes()
// {
// 	uint8_t ops[] = {
// 	};
// 	// load opcodes into memory
// 	set_memory_range(0x1000, ops, sizeof(ops));
// 	regs.pc = 0x1000;

// 	return true;
// }


// bool test_b_opcodes()
// {
// 	uint8_t ops[] = {
// 	};
// 	// load opcodes into memory
// 	set_memory_range(0x1000, ops, sizeof(ops));
// 	regs.pc = 0x1000;

// 	return true;
// }


bool test_c_opcodes()
{
	uint8_t ops[] = {
		0x18, // clc
		0xd8, // cld
		0x58, // cli
		0xb8, // clv

		0xc9, 0x01, // cmp #$ff
		0xc5, 0x10, // cmp $10
		0xd5, 0x10, // cmp $10, x
		0xcd, 0x00, 0x20, // cmp $2000
		0xdd, 0x00, 0x20, // cmp $2000, x
		0xd9, 0x00, 0x20, // cmp $2000, y
		0xc1, 0x10, // cmp ($12, x)
		0xd1, 0x12, // cmp ($xx), y

		0xe0, 0x01,
		0xe4, 0x10,
		0xec, 0x40, 0x20, // cpx $2040

		0xc0, 0x01,
		0xc4, 0x10,
		0xcc, 0x41, 0x20, // cpy $2041
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x10, 0x02);
	set_memory(0x11, 0x03);
	set_memory(0x12, 0x10);
	set_memory(0x13, 0x20);
	set_memory(0x2000, 0x50);
	set_memory(0x2001, 0x51);
	set_memory(0x2002, 0x52);
	set_memory(0x2010, 0x60);
	set_memory(0x2030, 0x70);
	set_memory(0x2040, 0x80);
	set_memory(0x2041, 0x90);
	regs.pc = 0x1000;

	// clear carry
	regs.status.carry = 0x1;
	cpu_step();
	ASSERT(regs.status.carry == 0x0, return false);

	// clear decimal mode
	regs.status.decimal = 0x1;
	cpu_step();
	ASSERT(regs.status.decimal == 0x0, return false);

	// clear interrupt
	regs.status.interrupt = 0x1;
	cpu_step();
	ASSERT(regs.status.interrupt == 0x0, return false);

	// clear overflow
	regs.status.overflow = 0x1;
	cpu_step();
	ASSERT(regs.status.overflow == 0x0, return false);

	/// cmp
	// setup to test <
	regs.acc = 0x00;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x0, return false);
	ASSERT(regs.status.zero == 0x0, return false);
	ASSERT(regs.status.negative == 0x1, return false);

	// setup to test ==
	regs.acc = 0x02;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test >
	regs.acc = 0x04;
	regs.x = 0x1;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x0, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test ==
	regs.acc = 0x50;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test ==
	regs.acc = 0x51;
	regs.x = 0x1;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test ==
	regs.acc = 0x52;
	regs.y = 0x2;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test ==
	regs.acc = 0x60;
	regs.x = 0x2;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test ==
	regs.acc = 0x70;
	regs.y = 0x20;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	/// cpx
	// setup to test <
	regs.x = 0x00;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x0, return false);
	ASSERT(regs.status.zero == 0x0, return false);
	ASSERT(regs.status.negative == 0x1, return false);

	// setup to test ==
	regs.x = 0x02;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test >
	regs.x = 0x81;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x0, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	/// cpy
	// setup to test <
	regs.y = 0x00;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x0, return false);
	ASSERT(regs.status.zero == 0x0, return false);
	ASSERT(regs.status.negative == 0x1, return false);

	// setup to test ==
	regs.y = 0x02;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x1, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	// setup to test >
	regs.y = 0x91;
	cpu_step();
	// verify
	ASSERT(regs.status.carry == 0x1, return false);
	ASSERT(regs.status.zero == 0x0, return false);
	ASSERT(regs.status.negative == 0x0, return false);

	return true;
}


bool test_d_opcodes()
{
	uint8_t ops[] = {
		0xc6, 0x10, // dec $10
		0xd6, 0x10, // dec $10, x
		0xce, 0x00, 0x20, // dec $2000
		0xde, 0x00, 0x20, // dec $2000,x
		0xca, // dex
		0xca, // dex
		0x88, // dey
		0x88, // dey
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x10, 0x01);
	set_memory(0x11, 0x00);
	set_memory(0x2000, 0x55);
	set_memory(0x2001, 0x5d);
	regs.pc = 0x1000;

	// setup
	cpu_step();
	// verify
	ASSERT(get_memory(0x10) == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.x = 1;
	cpu_step();
	// verify
	ASSERT(get_memory(0x11) == 0xff, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// setup
	cpu_step();
	// verify
	ASSERT(get_memory(0x2000) == 0x54, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.x = 1;
	cpu_step();
	// verify
	ASSERT(get_memory(0x2001) == 0x5c, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.x = 0x00;
	cpu_step();
	// verify
	ASSERT(regs.x == 0xff, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// setup
	regs.x = 0x01;
	cpu_step();
	// verify
	ASSERT(regs.x == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.y = 0x00;
	cpu_step();
	// verify
	ASSERT(regs.y == 0xff, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// setup
	regs.y = 0x01;
	cpu_step();
	// verify
	ASSERT(regs.y == 0x0, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	return true;
}

bool test_e_opcodes()
{
	uint8_t ops[] = {
		0x49, 0x55, // eor #$55
		0x45, 0x10, // eor $10
		0x55, 0x10, // eor $10, x
		0x4d, 0x00, 0x20, // eor $2000
		0x5d, 0x00, 0x20, // eor $2000,x
		0x59, 0x00, 0x20, // eor $2000,y
		0x41, 0x10, // eor ($10,x)
		0x51, 0x12  // eor ($12),y
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x10, 0x66);
	set_memory(0x11, 0x77);
	set_memory(0x12, 0x04);
	set_memory(0x13, 0x20);
	set_memory(0x2000, 0x22);
	set_memory(0x2001, 0x23);
	set_memory(0x2002, 0x24);
	set_memory(0x2004, 0x25);
	set_memory(0x2005, 0x26);
	regs.pc = 0x1000;

	// setup for immediate
	regs.acc = 0x55;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x0, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup for zero page
	regs.acc = 0x10;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x76, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup for zero page, x
	regs.acc = 0x10;
	regs.x = 0x1;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x67, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup for abs
	regs.acc = 0x10;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x32, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);
	
	// setup for abs,x
	regs.acc = 0x10;
	regs.x = 1;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x33, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);
	
	// setup for abs,y
	regs.acc = 0x10;
	regs.y = 2;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x34, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup for (ind,x)
	regs.acc = 0x10;
	regs.x = 2;
	cpu_step();
	// verify
	ASSERT(regs.acc == 0x35, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup for (ind),y
	regs.acc = 0x10;
	regs.y = 1;
	cpu_step();
	// // verify
	ASSERT(regs.acc == 0x36, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	return true;
}


bool test_i_opcodes()
{
	uint8_t ops[] = {
		0xe6, 0x10, // inc $10
		0xf6, 0x10, // inc $10, x
		0xee, 0x00, 0x20, // inc $2000
		0xfe, 0x00, 0x20, // inc $2000,x
		0xe8, // inx
		0xe8, // inx
		0xc8, // iny
		0xc8, // iny
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x10, 0xff);
	set_memory(0x11, 0x7f);
	set_memory(0x2000, 0x55);
	set_memory(0x2001, 0x5d);
	regs.pc = 0x1000;

	// setup
	cpu_step();
	// verify
	ASSERT(get_memory(0x10) == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.x = 1;
	cpu_step();
	// verify
	ASSERT(get_memory(0x11) == 0x80, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// setup
	cpu_step();
	// verify
	ASSERT(get_memory(0x2000) == 0x56, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.x = 1;
	cpu_step();
	// verify
	ASSERT(get_memory(0x2001) == 0x5e, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.x = 0x7f;
	cpu_step();
	// verify
	ASSERT(regs.x == 0x80, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// setup
	regs.x = 0xff;
	cpu_step();
	// verify
	ASSERT(regs.x == 0x0, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// setup
	regs.y = 0x7f;
	cpu_step();
	// verify
	ASSERT(regs.y == 0x80, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	// setup
	regs.y = 0xff;
	cpu_step();
	// verify
	ASSERT(regs.y == 0x0, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	return true;
}


// bool test_j_opcodes()
// {
// 	uint8_t ops[] = {
// 	};
// 	// load opcodes into memory
// 	set_memory_range(0x1000, ops, sizeof(ops));
// 	regs.pc = 0x1000;

// 	return true;
// }


bool test_l_opcodes()
{
	uint8_t ops[] = {
		0xa9, 0x00, // lda #$20
		0xa5, 0x20, // lda $20
		0xb5, 0x20, // lda $20, x
		0xad, 0x00, 0x20, // lda 0x2000
		0xbd, 0x00, 0x20, // lda 0x2000, x
		0xb9, 0x00, 0x20, // lda 0x2000, y
		0xa1, 0x21, // lda ($21, x)
		0xb1, 0x22, // lda ($22), y

		0xa2, 0x30, // ldx #$30
		0xa6, 0x30, // ldx $30
		0xb6, 0x30, // ldx $30, y
		0xae, 0x00, 0x30, // ldx $3000
		0xbe, 0x00, 0x30, // ldx $3000, y

		0xa0, 0x40, // ldy #$40
		0xa4, 0x40, // ldy $40
		0xb4, 0x40, // ldy $40, x
		0xac, 0x00, 0x40, // ldy $4000
		0xbc, 0x00, 0x40, // ldy $4000, x

		0x4a, // lsr acc
		0x46, 0x50, // lsr $50
		0x56, 0x50, // lsr %50, x
		0x4e, 0x00, 0x50, // lsr $5000
		0x5e, 0x00, 0x50, // lsr $5000, x
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x20, 0x21);
	set_memory(0x21, 0xff);
	set_memory(0x22, 0x10);
	set_memory(0x23, 0x20);
	set_memory(0x2000, 0x01);
	set_memory(0x2001, 0x02);
	set_memory(0x2002, 0x03);
	set_memory(0x2010, 0x31);
	set_memory(0x2011, 0x32);
	// ldx
	set_memory(0x30, 0x31);
	set_memory(0x31, 0x32);
	set_memory(0x3000, 0x33);
	set_memory(0x3001, 0x34);
	// ldy
	set_memory(0x40, 0x41);
	set_memory(0x41, 0x42);
	set_memory(0x4000, 0x43);
	set_memory(0x4001, 0x44);
	// lsr
	set_memory(0x50, 0x51);
	set_memory(0x51, 0x52);
	set_memory(0x5000, 0x55);
	set_memory(0x5001, 0x56);

	regs.pc = 0x1000;

	// lda
	cpu_step();
	ASSERT(regs.acc == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.acc == 0x21, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0xff, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	cpu_step();
	ASSERT(regs.acc == 0x01, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0x02, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.y = 0x2;
	cpu_step();
	ASSERT(regs.acc == 0x03, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0x31, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.y = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0x32, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// ldx
	cpu_step();
	ASSERT(regs.x == 0x30, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.x == 0x31, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.y = 0x1;
	cpu_step();
	ASSERT(regs.x == 0x32, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.x == 0x33, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.x == 0x34, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// ldy
	cpu_step();
	ASSERT(regs.y == 0x40, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.y == 0x41, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.y == 0x42, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.y == 0x43, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.y == 0x44, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	// lsr acc
	regs.acc = 0x3;
	cpu_step();
	ASSERT(regs.acc == 0x1, return false);
	ASSERT(regs.status.carry == 1, return false);

	cpu_step();
	ASSERT(get_memory(0x50) == 0x28, return false);
	ASSERT(regs.status.carry == 1, return false);

	regs.x = 0x1;
	cpu_step();
	ASSERT(get_memory(0x51) == 0x29, return false);
	ASSERT(regs.status.carry == 0, return false);

	cpu_step();
	ASSERT(get_memory(0x5000) == 0x2a, return false);
	ASSERT(regs.status.carry == 1, return false);

	regs.x = 0x1;
	cpu_step();
	ASSERT(get_memory(0x5001) == 0x2b, return false);
	ASSERT(regs.status.carry == 0, return false);

	return true;
}


bool test_no_opcodes()
{
	uint8_t ops[] = {
		0xea, // nop
		0x09, 0x70, // ora #$70
		0x05, 0x50, // ora $50
		0x15, 0x50, // ora $50, x
		0x0d, 0x00, 0x20, // ora $2000
		0x1d, 0x00, 0x20, // ora $2000, x
		0x19, 0x00, 0x20, // ora $2000, y
		0x01, 0x60, // ora ($60, x)
		0x11, 0x61, // ora ($61), y
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x50, 0x0);
	set_memory(0x51, 0xaa);
	set_memory(0x61, 0x00);
	set_memory(0x62, 0x30);
	set_memory(0x2000, 0xb0);
	set_memory(0x2001, 0xc0);
	set_memory(0x2002, 0xd0);
	set_memory(0x3000, 0x30);
	set_memory(0x3030, 0x40);
	regs.pc = 0x1000;

	cpu_step(); // nothing for nop

	// ora
	regs.acc = 0x5;
	cpu_step();
	ASSERT(regs.acc == 0x75, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.acc = 0x0;
	cpu_step();
	ASSERT(regs.acc == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.acc = 0x55;
	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0xff, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.acc = 0xb;
	cpu_step();
	ASSERT(regs.acc == 0xbb, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.acc = 0xc;
	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0xcc, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.acc = 0xd;
	regs.y = 0x2;
	cpu_step();
	ASSERT(regs.acc == 0xdd, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.acc = 0x03;
	regs.x = 0x1;
	cpu_step();
	ASSERT(regs.acc == 0x33, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.acc = 0x04;
	regs.y = 0x30;
	cpu_step();
	ASSERT(regs.acc == 0x44, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	return true;
}


bool test_p_opcodes()
{
	uint8_t ops[] = {
		0x48, // pha
		0x08, // php
		0x08,
		0x68, // pla
		0x68,
		0x68,
		0x28, // plp
		0x28
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x40, 0x50);
	set_memory(0x41, 0x80);
	set_memory(0x42, 0x00);
	set_memory(0x50, 0x55);
	set_memory(0x51, 0x8a);
	regs.pc = 0x1000;

	// pha
	regs.acc = 0x11;
	regs.sp = 0x30;
	cpu_step();
	ASSERT(get_memory(0x30) == 0x11, return false);
	ASSERT(regs.sp == 0x2f, return false);

	// php
	set_proc_status(0x8a); // 1 0 - 0 1 0 1 0
	regs.sp = 0x30;
	cpu_step();
	ASSERT(get_proc_status() == 0x8a, return false);
	ASSERT(regs.sp == 0x2f, return false);

	set_proc_status(0x55); // 0 1 - 1 0 1 0 1
	regs.sp = 0x30;
	cpu_step();
	ASSERT(get_proc_status() == 0x55, return false);
	ASSERT(regs.sp == 0x2f, return false);

	// pla
	regs.sp = 0x3f;
	cpu_step();
	ASSERT(regs.acc == 0x50, return false);
	ASSERT(regs.sp == 0x40, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.acc == 0x80, return false);
	ASSERT(regs.sp == 0x41, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	cpu_step();
	ASSERT(regs.acc == 0x00, return false);
	ASSERT(regs.sp == 0x42, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// plp
	regs.sp = 0x4f;
	cpu_step();
	ASSERT(regs.sp == 0x50, return false);
	ASSERT(regs.status.carry == 1, return false);  // 0 1 - 1 0 1 0 1
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.interrupt == 1, return false);
	ASSERT(regs.status.decimal == 0, return false);
	ASSERT(regs.status.break_cmd == 1, return false);
	ASSERT(regs.status.overflow == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	cpu_step();
	ASSERT(regs.sp == 0x51, return false);
	ASSERT(regs.status.carry == 0, return false);  // 1 0 - 0 1 0 1 0
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.interrupt == 0, return false);
	ASSERT(regs.status.decimal == 1, return false);
	ASSERT(regs.status.break_cmd == 0, return false);
	ASSERT(regs.status.overflow == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	return true;
}


// bool test_r_opcodes()
// {
// 	uint8_t ops[] = {
// 	};
// 	// load opcodes into memory
// 	set_memory_range(0x1000, ops, sizeof(ops));
// 	regs.pc = 0x1000;

// 	return true;
// }


bool test_s_opcodes()
{
	uint8_t ops[] = {
		// TODO finish
		// 0xe9, 0xxx, // sbc #$xxxx
		// 0xe5, 0x10, // sbc $10
		// 0xf5, 0x10, // sbc $10,x
		// 0xed, 0x00, 0x10, // sbc $1000
		// 0xfd, 0x00, 0x10, // sbc $1000, x
		// 0xf9, 0x00, 0x10, // sbc $1000, y
		// 0xe1, 0xxx,// sbc ($xx,x)
		// 0xf1, 0xxx,// sbc ($xx),y

		0x38, // sec
		0xf8, // sed
		0x78, // sei

		0x85, 0x30, // sta $30
		0x95, 0x30, // sta $30, x
		0x8d, 0x00, 0x20, // sta $2000
		0x9d, 0x00, 0x20, // sta $2000, x
		0x99, 0x00, 0x20, // sta $2000, y
		0x81, 0x40, // sta ($40,x)
		0x91, 0x40, // sta ($40),y

		0x86, 0x50, // stx $50
		0x96, 0x50, // stx $50, y
		0x8e, 0x00, 0x30, // stx $3000

		0x84, 0x60, // sty $60
		0x94, 0x60, // sty $60, x
		0x8c, 0x00, 0x40, // sty $4000
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	set_memory(0x40, 0x00);
	set_memory(0x41, 0x20);
	set_memory(0x43, 0x03);
	set_memory(0x44, 0x20);
	regs.pc = 0x1000;

	// carry
	regs.status.carry = 0;
	cpu_step();
	ASSERT(regs.status.carry == 1, return false);

	// decimal
	regs.status.decimal = 0;
	cpu_step();
	ASSERT(regs.status.decimal == 1, return false);

	// interrupt
	regs.status.interrupt = 0;
	cpu_step();
	ASSERT(regs.status.interrupt == 1, return false);

	// sta zpage
	regs.acc = 0x55;
	cpu_step();
	ASSERT(get_memory(0x30) == 0x55, return false);

	// sta zpage,x
	regs.x = 0x1;
	regs.acc = 0x56;
	cpu_step();
	ASSERT(get_memory(0x31) == 0x56, return false);

	//
	regs.acc = 0x57;
	cpu_step();
	ASSERT(get_memory(0x2000) == 0x57, return false);

	//
	regs.x = 0x1;
	regs.acc = 0x58;
	cpu_step();
	ASSERT(get_memory(0x2001) == 0x58, return false);

	//
	regs.y = 0x2;
	regs.acc = 0x59;
	cpu_step();
	ASSERT(get_memory(0x2002) == 0x59, return false);

	// sta indirect, x
	regs.x = 0x3;
	regs.acc = 0x5a;
	cpu_step();
	ASSERT(get_memory(0x2003) == 0x5a, return false);

	// sta indirect, y
	regs.y = 0x4;
	regs.acc = 0x5b;
	cpu_step();
	ASSERT(get_memory(0x2004) == 0x5b, return false);

	// stx zpage
	regs.x = 0x60;
	cpu_step();
	ASSERT(get_memory(0x50) == 0x60, return false);

	// stx zpage,y
	regs.y = 0x1;
	regs.x = 0x61;
	cpu_step();
	ASSERT(get_memory(0x51) == 0x61, return false);

	// stx absolute
	regs.x = 0x62;
	cpu_step();
	ASSERT(get_memory(0x3000) == 0x62, return false);

	// sty zpage
	regs.y = 0x70;
	cpu_step();
	ASSERT(get_memory(0x60) == 0x70, return false);

	// sty zpage,x
	regs.x = 0x1;
	regs.y = 0x71;
	cpu_step();
	ASSERT(get_memory(0x61) == 0x71, return false);

	// sty absolute
	regs.y = 0x72;
	cpu_step();
	ASSERT(get_memory(0x4000) == 0x72, return false);

	return true;
}


bool test_t_opcodes()
{
	uint8_t ops[] = {
		0xaa, // tax
		0xaa,
		0xaa,
		0xa8, // tay
		0xa8,
		0xa8,
		0x8a, // txa
		0x8a,
		0x8a,
		0x98, // tya
		0x98,
		0x98,
		0xba, // tsx
		0xba,
		0xba,
		0x9a, // txs
		0x9a,
		0x9a
	//tsx: transfer sp to x
	//txs: transfer x to sp
	};
	// load opcodes into memory
	set_memory_range(0x1000, ops, sizeof(ops));
	regs.pc = 0x1000;

	// test tax
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x50;
	cpu_step();
	ASSERT(regs.x == 0x50, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x80;
	cpu_step();
	ASSERT(regs.x == 0x80, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x00;
	cpu_step();
	ASSERT(regs.x == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// test tay
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x51;
	cpu_step();
	ASSERT(regs.y == 0x51, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x85;
	cpu_step();
	ASSERT(regs.y == 0x85, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.acc = 0x00;
	cpu_step();
	ASSERT(regs.y == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// txa
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.x = 0x52;
	cpu_step();
	ASSERT(regs.acc == 0x52, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.x = 0x82;
	cpu_step();
	ASSERT(regs.acc == 0x82, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.x = 0x00;
	cpu_step();
	ASSERT(regs.acc == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// tya
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.y = 0x53;
	cpu_step();
	ASSERT(regs.acc == 0x53, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.y = 0x83;
	cpu_step();
	ASSERT(regs.acc == 0x83, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.y = 0x00;
	cpu_step();
	ASSERT(regs.acc == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	// tsx
	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.sp = 0x54;
	cpu_step();
	ASSERT(regs.x == 0x54, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 0, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.sp = 0x84;
	cpu_step();
	ASSERT(regs.x == 0x84, return false);
	ASSERT(regs.status.zero == 0, return false);
	ASSERT(regs.status.negative == 1, return false);

	regs.status.zero = 0;
	regs.status.negative = 0;
	regs.sp = 0x00;
	cpu_step();
	ASSERT(regs.x == 0x00, return false);
	ASSERT(regs.status.zero == 1, return false);
	ASSERT(regs.status.negative == 0, return false);

	return true;
}


int main(int argc, char** argv)
{
	cpu_init();

	if (/*test_addrmode_immediate()
	    && test_addrmode_zero_page()
	    && test_addrmode_zero_page_x()
	    && test_addrmode_zero_page_y()
	    && test_addrmode_abs()
	    && test_addrmode_abs_x()
	    && test_addrmode_abs_y()
	    && test_addrmode_indirect()
	    && test_addrmode_indexed_indirect_x()
	    && test_addrmode_indirect_indexed_y()
	    //&& test_a_opcodes()
	    //&& test_b_opcodes()
	    &&*/ test_c_opcodes()
	    && test_d_opcodes()
	    && test_e_opcodes()
	    && test_i_opcodes()
	    //&& test_j_opcodes()
	    && test_l_opcodes()
	    && test_no_opcodes()
	    && test_p_opcodes()
	    //&& test_r_opcodes()
	    && test_s_opcodes()
	    && test_t_opcodes()
	    )
	{
		printf("test result: success\n");
		return true;
	}
	else
	{
		printf("test result: failed\n");
		return false;
	}
}
