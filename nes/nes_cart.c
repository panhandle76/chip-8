
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> //malloc

/**
 * Sample RAM map for games
$0000-$000F	16 bytes	Local variables and function arguments
$0010-$00FF	240 bytes	Global variables accessed most often, including certain pointer tables
$0100-$019F	160 bytes	Data to be copied to nametable during next vertical blank (see The frame and NMIs)
$01A0-$01FF	96 bytes	Stack
$0200-$02FF	256 bytes	Data to be copied to OAM during next vertical blank
$0300-$03FF	256 bytes	Variables used by sound player, and possibly other variables
$0400-$07FF	1024 bytes	Arrays and less-often-accessed global variables
 */

/*
An iNES file consists of the following sections, in order:

1. Header (16 bytes)
2. Trainer, if present (0 or 512 bytes)
3. PRG ROM data (16384 * x bytes)
4. CHR ROM data, if present (8192 * y bytes)
5. PlayChoice INST-ROM, if present (0 or 8192 bytes)
6. PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing, see PC10 ROM-Images for details)

Some ROM-Images additionally contain a 128-byte (or sometimes 127-byte) title at the end of the file.

The format of the header is as follows:

0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
4: Size of PRG ROM in 16 KB units
5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
6: Flags 6
7: Flags 7
8: Size of PRG RAM in 8 KB units (Value 0 infers 8 KB for compatibility; see PRG RAM circuit)
9: Flags 9
10: Flags 10 (unofficial)
11-15: Zero filled
 */


uint8_t header[16];
uint8_t trainer[512];
uint8_t *prg_rom = NULL;
uint8_t *chr_rom = NULL;
uint8_t *inst_rom = NULL;
uint8_t *prom = NULL;
uint8_t *title = NULL;

//
uint8_t prg_rom_blksz = 0;
uint8_t chr_rom_blksz = 0;
uint8_t prg_ram_blksz = 0;

// flags
uint8_t mirror_flag = 0;
uint8_t prg_ram_flag = 0;
uint8_t trainer_flag = 0;


/**
 * returns: 0 on success
 */
uint8_t nes_read(const char *filepath)
{
	//uint8_t rtn = 0;
	printf("%s: filepath=%s\n", __func__, filepath);

	FILE* f = (FILE*) fopen(filepath, "r");
	if(f != NULL) // if pointer is valid
	{
		// read the header
		printf("%s: read header\n", __func__);
		if (fread(header, sizeof(uint8_t), 16, f) != 16)
		{
			fclose(f);
			return 1;
		}

		// is this the right file format
		if (header[0] != 0x4e || 
			header[1] != 0x45 ||
			header[2] != 0x53 ||
			header[3] != 0x1a)
		{
			fclose(f);
			return 2;
		}

		prg_rom_blksz = header[4];
		chr_rom_blksz = header[5];

		// read flags from header
		mirror_flag  = (header[6] & 0x01) ? 1 : 0;
		prg_ram_flag = (header[6] & 0x02) ? 1 : 0;
		trainer_flag = (header[6] & 0x04) ? 1 : 0;
		printf("mirror:%u prg_ram:%u trainer:%u\n", mirror_flag, prg_ram_flag, trainer_flag);

		prg_ram_blksz = header[7];
		
		// check flag 6 to see if trainer is available
		if (trainer_flag)
		{
			printf("%s: read trainer 512 bytes\n", __func__);
			fread(trainer, sizeof(uint8_t), 512, f);
		}

		// 
		if (prg_rom_blksz > 0)
		{
			printf("%s: PRG ROM: %u blocks\n", __func__, prg_rom_blksz);
			prg_rom = (uint8_t*) malloc(prg_rom_blksz*(16*1024));
			fread(prg_rom, sizeof(uint8_t), prg_rom_blksz*(16*1024), f);
		}

		if (chr_rom_blksz > 0)
		{
			printf("%s: CHR ROM: %u blocks\n", __func__, chr_rom_blksz);
			chr_rom = (uint8_t*) malloc(chr_rom_blksz*(8*1024));
			fread(chr_rom, sizeof(uint8_t), chr_rom_blksz*(8*1024), f);
		}

		printf("%s: close file\n", __func__);
		fclose(f);
	}

	return 0;
}

uint8_t nes_prg_blksz()
{
	return prg_rom_blksz;
}

const uint8_t* nes_prg_rom()
{
	return prg_rom;
}

const uint8_t* nes_chr_rom()
{
	return chr_rom;
}