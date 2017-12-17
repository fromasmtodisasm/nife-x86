#include <stdio.h>
#include <string.h>

#include "exe.h"
#include "elf.h"
#include "disassemble.h"

const char* TEXT = ".text";

int test_types()
{
  if(sizeof(uint8) != 1)
    return -1;

  if(sizeof(uint16) != 2)
    return -1;

  if(sizeof(uint32) != 4)
    return -1;

  if(sizeof(int8) != 1)
    return -1;

  if(sizeof(int16) != 2)
    return -1;

  if(sizeof(int32) != 4)
    return -1;

  return 0;
}

void usage(char** argv)
{
  printf("USAGE: %s [--usage|FILENAME]\n", argv[0]);
}

int handle(const char* arg)
{
  printf("\n--- ASSUMING EXE FILE ---\n");
  exe_t* exe = parse_exe(arg);

  if(!exe)
    goto next_try;

  int nsections = exe->coff_header->nsections;
  int section;

  for(section = 0; section < nsections; section++)
  {
    if(!strcmp(&exe->section_headers[section].name[0], TEXT))
    {
      int begin = exe->section_headers[section].pointer_to_rawdata;
      int end   = begin + exe->section_headers[section].size_of_rawdata - 1;

      disassemble(exe->raw_file, begin, end);
    }
  }

  free_exe(exe);

  return 0;
next_try:

  printf("\n--- ASSUMING ELF FILE ---\n");
  elf_t* elf = parse_elf(arg);

  if(!elf)
    return -1;

  nsections = elf->elf_header->shnum;
  section   = 0;

  for(; section < nsections; section++)
  {
    if(!strcmp(elf_get_section_name(elf, section), TEXT))
    {
      int begin = elf->section_headers[section].offset;
      int end   = begin + elf->section_headers[section].size - 1;

      disassemble(elf->raw_file, begin, end);
    }
  } 

  free_elf(elf);

  return 0;
}

int main(int argc, char** argv)
{
  if(test_types())
  {
    printf("sizeof(uint8) = %d\n", sizeof(uint8));
    printf("sizeof(uint16) = %d\n", sizeof(uint16));
    printf("sizeof(uint32) = %d\n", sizeof(uint32));

    printf("sizeof(int8) = %d\n", sizeof(int8));
    printf("sizeof(int16) = %d\n", sizeof(int16));
    printf("sizeof(int32) = %d\n", sizeof(int32));

    return -1;
  }

  if(argc != 2)
  {
    usage(argv);

    return -1;
  }
  else if(!strcmp(argv[1], "--usage"))
  {
    usage(argv);
  }
  else
  {
    return handle(argv[1]);
  }

  return 0;
}
