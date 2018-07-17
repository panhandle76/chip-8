#include "cpu-6502.h"
#include "nes_cart.h"
#include <stdio.h>
#include <string.h>


void printHelp(char* app)
{
   printf("Usage: %s [-?hde] FILE\n", app);
   printf(" ?\tDisplay this help menu\n");
   printf(" h\tPerform hex dump\n");
   printf(" d\tPerform disassembly\n");
   printf(" e\tPerform emulation\n");
   printf("\n");
}


void hexdump(const uint8_t* binary, int length)
{
   int address = 0;
 
   printf("HEXDUMP %i bytes\n", length);
   
   if((binary == NULL) || (length <= 0))
      return;
   
   for(int i=0; i<length; i++)
   {
      if((i%16)==0)
      {
         printf("%07x ", address);
         address += 0x10;
      }
      
      printf("%02x ", binary[i]);
      
      // if end of line OR last byte
       if( (((i+1)%16)==0) || ((i+1)==length) )
       {
          printf("\n");
       }
   }
}


int main(int argc, char* argv[])
{
   bool dump=false;
   bool diss=false;
   bool emulate=false;
   
   if(argc<3)
   {
      printHelp(argv[0]);
      return 0;
   }
   
   // validate options
   if(argv[1][0] == '-')
   {
      if( strstr(argv[1], "?") != NULL )
      {
         printHelp(argv[0]);
         return 0;
      }
         
      if( strstr(argv[1], "h") != NULL )
         dump=true;
      
      if( strstr(argv[1], "d") != NULL )
         diss=true;
      
      if( strstr(argv[1], "e") != NULL )
         emulate=true;
   }
   else
   {
      printf("invalid option\n");
      printHelp(argv[0]);
      return -1;
   }
   
   // FILE* f = (FILE*) fopen(argv[2], "r");
   // if(f != NULL) // if pointer is valid
   // {
      // how big is file
      // fseek(f, 0, SEEK_END);
      // int fsize = ftell(f);
      // fseek(f, 0, SEEK_SET);
      
      // read in whole file
      // uint8_t* binary = (uint8_t*) malloc(fsize+1);
      // fread(binary, fsize, sizeof(uint8_t), f);
      // fclose(f); // close file


      // read in the nes file
      uint8_t rtn = nes_read(argv[2]);
      if (rtn)
        printf("nes_read failed with error %u\n", rtn);
      const uint8_t *binary = nes_prg_rom();
      uint8_t prg_blksz = nes_prg_blksz();
      int fsize = prg_blksz*(16*1024);
      
      cpu_init();
      cpu_load_prg(binary, prg_blksz);
      cpu_reset();
      cpu_run();

      // hexdump
      if(dump)
         hexdump(binary, fsize);
      
      //Machine mach;
      // disassemble
      //int pc = 0;
      if(diss)
      {
      //   for (int i=0xd70; i < fsize; )
      //   {
      //     if (opcode_len[binary[i]] > 0)
      //     {
      //       printf("%s\n", opcode_name[binary[i]]);
      //       i+=opcode_len[binary[i]];
      //     }
      //     else
      //     {
      //       printf("na\n");
      //       i++;
      //     }
      //    //mach.disassemble(binary, fsize);
      //   }
      }
      
      // emulate
      if(emulate)
      {
         //mach.execute(binary, fsize);
      }
      
      // cleanup memory
      // free(binary);
   // }
   
   return 0;
}