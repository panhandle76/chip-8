/*
Information pulled from https://en.wikipedia.org/wiki/MOS_Technology_6502

CPU Info:man 
1.79 MHz
8 bit

Registers:
15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
                        |          A          | Accumulator
                        |          X          | X index
                        |          Y          | Y index
| 0  0  0  0  0  0  0  1|         SP          | Stack Pointer
|                       PC                    | Program Counter
                        |  N V - B D I Z C    | Processor flags

8-bit Accumulator register
The Accumulator (A) is the main 8 bit register for loading, storing, comparing, and doing math on data.
two 8-bit index registers (X and Y)
The Index Register X (X) is another 8 bit register, usually used for counting or memory access. In loops you will use this register to keep track of how many times the loop has gone, while using A to process data.
The Index Register Y (Y) works almost the same as X. Some instructions (not covered here) only work with X and not Y.
The Status Register holds flags with information about the last instruction. For example when doing a subtract you can check if the result was a zero. 
7 processor status flag bits (P)
8-bit stack pointer
16-bit program counter

stack address space is hardwared to memory page $01, ie the address
range 0x0100-0x01FF (256-511)

Addressing:
16 bit address space (64KB ram) (NES only has 2KB ram)

-no multiple or divide instructions
-no floating point

NES memory map:
0x0000-0x0799 :RAM
0x0800-0x0FFF :PPU/APU/Game Cartridge/input devices/etc
0x1000-0x1800 :Mirrors RAM since some address lines are unwired on NES

$0000-0800 - Internal RAM, 2KB chip in the NES
$2000-2007 - PPU access ports
$4000-4017 - Audio and controller access ports
$6000-7FFF - Optional WRAM inside the game cart
$8000-FFFF - Game cart ROM

opcodes:
length 1-3 bytes
little-endian format


Common Load/Store opcodes
LDA #$0A   ; LoaD the value 0A into the accumulator A
           ; the number part of the opcode can be a value or an address
           ; if the value is zero, the zero flag will be set.

LDX $0000  ; LoaD the value at address $0000 into the index register X
           ; if the value is zero, the zero flag will be set.

LDY #$FF   ; LoaD the value $FF into the index register Y
           ; if the value is zero, the zero flag will be set.

STA $2000  ; STore the value from accumulator A into the address $2000
           ; the number part must be an address

STX $4016  ; STore value in X into $4016
           ; the number part must be an address

STY $0101  ; STore Y into $0101
           ; the number part must be an address

TAX        ; Transfer the value from A into X
           ; if the value is zero, the zero flag will be set

TAY        ; Transfer A into Y
           ; if the value is zero, the zero flag will be set

TXA        ; Transfer X into A
           ; if the value is zero, the zero flag will be set

TYA        ; Transfer Y into A
           ; if the value is zero, the zero flag will be set


Common Math opcodes
ADC #$01   ; ADd with Carry
           ; A = A + $01 + carry
           ; if the result is zero, the zero flag will be set

SBC #$80   ; SuBtract with Carry
           ; A = A - $80 - (1 - carry)
           ; if the result is zero, the zero flag will be set

CLC        ; CLear Carry flag in status register
           ; usually this should be done before ADC

SEC        ; SEt Carry flag in status register
           ; usually this should be done before SBC

INC $0100  ; INCrement value at address $0100
           ; if the result is zero, the zero flag will be set

DEC $0001  ; DECrement $0001
           ; if the result is zero, the zero flag will be set

INY        ; INcrement Y register
           ; if the result is zero, the zero flag will be set

INX        ; INcrement X register
           ; if the result is zero, the zero flag will be set

DEY        ; DEcrement Y
           ; if the result is zero, the zero flag will be set

DEX        ; DEcrement X
           ; if the result is zero, the zero flag will be set

ASL A      ; Arithmetic Shift Left
           ; shift all bits one position to the left
           ; this is a multiply by 2
           ; if the result is zero, the zero flag will be set

LSR $6000  ; Logical Shift Right
           ; shift all bits one position to the right
           ; this is a divide by 2
           ; if the result is zero, the zero flag will be set


Common Comparison opcodes
CMP #$01   ; CoMPare A to the value $01
           ; this actually does a subtract, but does not keep the result
           ; instead you check the status register to check for equal, 
           ; less than, or greater than

CPX $0050  ; ComPare X to the value at address $0050

CPY #$FF   ; ComPare Y to the value $FF


Common Control Flow opcodes
JMP $8000  ; JuMP to $8000, continue running code there

BEQ $FF00  ; Branch if EQual, contnue running code there
           ; first you would do a CMP, which clears or sets the zero flag
           ; then the BEQ will check the zero flag
           ; if zero is set (values were equal) the code jumps to $FF00 and runs there
           ; if zero is clear (values not equal) there is no jump, runs next instruction

BNE $FF00  ; Branch if Not Equal - opposite above, jump is made when zero flag is clear



*/



