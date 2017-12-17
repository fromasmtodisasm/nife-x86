#ifndef __ELF_H__
#define __ELF_H__

#include "common.h"

#define ELF_IDENT_SIZE 16

#define MAGIC0 0x7F
#define MAGIC1 'E'
#define MAGIC2 'L'
#define MAGIC3 'F'

#define ELF_INTEL386 3 // Yes, the value so simple it scares me.

typedef struct
{
  uint8  ident[ELF_IDENT_SIZE];
  uint16 type;
  uint16 machine;
  uint32 version;
  uint32 entry;
  uint32 phoff;
  uint32 shoff;
  uint32 flags;
  uint16 ehsize;
  uint16 phentsize;
  uint16 phnum;
  uint16 shentsize;
  uint16 shnum;
  uint16 shstrndx;
} __attribute__((packed)) elf_header_t;

typedef struct
{
  uint32 name;
  uint32 type;
  uint32 flags;
  uint32 addr;
  uint32 offset;
  uint32 size;
  uint32 link;
  uint32 info;
  uint32 addralign;
  uint32 entsize;
} __attribute__((packed)) elf_section_header_t;

typedef struct
{
  uint8* raw_file;

  elf_header_t*         elf_header;
  elf_section_header_t* section_headers;
} elf_t;

char*  elf_get_section_name(elf_t* elf, int section);

elf_t* parse_elf(const char* filename);
void   free_elf(elf_t* elf);

#endif
