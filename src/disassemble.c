#include <disassemble.h>

#define MOD(X) (((X) & 0xC0) >> 6)
#define REG(X) ((X) & 0x07)

const char* rtable8[] =
{
  "AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH" 
};

const char* rtable16[] = 
{
  "AX", "CX", "DX", "SP", "BP", "SI", "DI"
};

const char* rtable32[] = 
{
  "EAX", "ECX", "EBX", "ESP", "ESI", "EDI"
};

static int print_opcode(uint8 opcode)
{
  switch(opcode)
  {
    case 0x88:
      printf("MOV ");
      return 8;

    default:
      printf("UNKNOWN");
      return -1;
  }
}

static void print_modrm(uint8 modrm, int regsize)
{
  if(MOD(modrm) == 3)
  {
    switch(regsize)
    {
      case 8:
        printf(rtable8[REG(modrm)]);
	break;

      case 16:
        printf(rtable16[REG(modrm)]);
	break;

      case 32:
        printf(rtable32[REG(modrm)]);
	break;

      default:
	break;
    }
  }
}

void disassemble(uint8* code, int begin, int end)
{
  int p = begin;

  for(; p < end; p++)
  {
    uint8 opcode = code[p];
    uint8 modrm = code[p+1];
    p += 2;

    int rs = print_opcode(opcode);
    print_modrm(modrm, rs);
    printf("\n");
  }  
}
