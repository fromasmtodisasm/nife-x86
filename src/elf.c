#include "elf.h"

char* elf_get_section_name(elf_t* elf, int section)
{
  int strtab = (void*)elf->section_headers[elf->elf_header->shstrndx].offset;
  strtab += elf->section_headers[section].name;

  return (void*)&elf->raw_file[strtab];
}

elf_t* parse_elf(const char* filename)
{
  elf_t* elf = malloc(sizeof(elf_t));
  elf->raw_file = read_whole_file(filename);

  elf->elf_header = (void*)elf->raw_file;

  if(elf->elf_header->ident[0] != MAGIC0)
    goto magic_err;

  if(elf->elf_header->ident[1] != MAGIC1)
    goto magic_err;

  if(elf->elf_header->ident[2] != MAGIC2)
    goto magic_err;

  if(elf->elf_header->ident[3] != MAGIC3)
    goto magic_err;

  if(elf->elf_header->machine != ELF_INTEL386)
  {
    printf("Bad architecture.\n");
    goto common_err;
  }

  if(!elf->elf_header->type)
  {
    printf("The file is not executable nor object.\n");
    goto common_err;
  }

  elf->section_headers = (void*)&elf->raw_file[elf->elf_header->shoff];

  int section = 0;
  for(; section < elf->elf_header->shnum; section++)
  {
    int begin = elf->section_headers[section].offset;
    int end   = begin + elf->section_headers[section].size;

    printf("Section #%d. %s %d %d\n", section,
		                      elf_get_section_name(elf, section),
				      begin,
				      end);
  }

  return elf;

magic_err:
  printf("This file is not ELF!\n");
common_err:
  free_elf(elf);
  return 0;
}

void free_elf(elf_t* elf)
{
  DELETE(elf->raw_file);
  DELETE(elf);
}
