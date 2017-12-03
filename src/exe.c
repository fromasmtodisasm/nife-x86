#include <stdio.h>
#include <stdlib.h>

#include "exe.h"

static uint8 MZ[] = {'M', 'Z'};
static uint8 PE[] = {'P', 'E', 0, 0};

static int invalid_signature(uint8* signature, uint8* pattern, int size)
{
  int i;

  for(i = 0; i < size; i++)
  {
    if(signature[i] != pattern[i])
      return 1;
  }

  return 0;
}

static uint8* read_whole_file(const char* name)
{
  FILE* file = fopen(name, "rb");
  fseek(file, 0, SEEK_END);
  uint32 size = ftell(file);
  fseek(file, 0, SEEK_SET);

  uint8* buf = malloc(size);
  fread(buf, size, 1, file);
  fclose(file);

  printf("Loaded file size: %d bytes\n", size);

  return buf;
}

exe_t* parse_exe(const char* name)
{
  exe_t* exe = malloc(sizeof(exe_t));

  exe->raw_file           = read_whole_file(name);
  exe->dos_header         = NULL;
  exe->pe_header          = NULL;
  exe->coff_header        = NULL;
  exe->pe_optional_header = NULL;

  exe->dos_header = malloc(sizeof(dos_header_t));
  memcpy(exe->dos_header, exe->raw_file, sizeof(dos_header_t));

  if(invalid_signature(exe->dos_header->signature, MZ, 2))
  {
    printf("The file is not EXE!\n");
    free_exe(exe);

    return NULL;
  }

  exe->pe_header   = malloc(sizeof(pe_header_t));
  exe->coff_header = malloc(sizeof(coff_header_t));
  exe->pe_optional_header = malloc(sizeof(pe_optional_header_t));

  int addr = exe->dos_header->e_lfanew;
  memcpy(exe->pe_header, &exe->raw_file[addr], sizeof(pe_header_t));

  addr += sizeof(pe_header_t);
  memcpy(exe->coff_header, &exe->raw_file[addr], sizeof(coff_header_t));

  addr += sizeof(coff_header_t);
  memcpy(exe->pe_optional_header, &exe->raw_file[addr], sizeof(pe_optional_header_t));

  if(invalid_signature(exe->pe_header->signature, PE, 4))
  {
    printf("The file is not PE!\n");
    free_exe(exe);

    return NULL;
  }

  if(exe->coff_header->machine != INTEL386)
  {
    printf("Architecture is not supported!\n");
    free_exe(exe);

    return NULL;
  }

  if(exe->coff_header->characteristics & IMAGE_FILE_EXECUTABLE_IMAGE != IMAGE_FILE_EXECUTABLE_IMAGE)
  {
    printf("The file is not executable.\n");
    printf("Possibly you are trying to pass objective file to the NifeX86 disassembler.\n");
    free_exe(exe);

    return NULL;
  }

  return exe;
}

void free_exe(exe_t* exe)
{
  free(exe->raw_file);

  DELETE(exe->dos_header);
  DELETE(exe->pe_header);
  DELETE(exe->coff_header);

  free(exe);
}
