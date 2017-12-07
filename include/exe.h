#ifndef __EXE_H__
#define __EXE_H__

#include "common.h"

#define INTEL386                      0x014c
#define IMAGE_FILE_EXECUTABLE_IMAGE   0x0002

#define IMAGE_SUBSYSTEM_NATIVE        0x0001
#define IMAGE_SUBSYSTEM_WINGUI        0x0002
#define IMAGE_SUBSYSTEM_WINCUI        0x0003

typedef struct          // OFFSET
{                       // DEC   HEX
  uint8  signature[2];  // 0     0
  uint16 lastsize;      // 2     2
  uint16 nblocks;       // 4     4
  uint16 nreloc;        // 6     6
  uint16 hdrsize;       // 8     8
  uint16 minalloc;      // 10    A
  uint16 maxalloc;      // 12    C
  uint16 ss;            // 14    D
  uint16 sp;            // 16    10
  uint16 checksum;      // 18    12
  uint16 ip;            // 20    14
  uint16 cs;            // 22    16
  uint16 relocops;      // 24    18
  uint16 noverlay;      // 26    1A
  uint16 reserved1[4];  // 28    1C
  uint16 oem_id;        // 36    24
  uint16 oem_info;      // 38    26
  uint16 reserved2[10]; // 40    28

  uint32 e_lfanew;      // 60    3C
} __attribute__((packed)) dos_header_t;

typedef struct
{
  uint8 signature[4];
} __attribute__((packed)) pe_header_t;

typedef struct
{
  uint16 machine;
  uint16 nsections;
  uint32 time_date_stamp;
  uint32 pointer_to_sybmol_table;
  uint32 nsymbols;
  uint16 size_of_optional_header;
  uint16 characteristics;
} __attribute__((packed)) coff_header_t;

typedef struct
{
  uint32 va;
  uint32 size;
} __attribute__((packed)) data_directory_t;

typedef struct
{
  uint16 signature;
  uint8  major_linker_version;
  uint8  minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;
  uint32 base_of_code;
  uint32 base_of_data;
  uint32 image_base;
  uint32 section_alignment;
  uint32 file_alignment;
  uint16 major_osversion;
  uint16 minor_osversion;
  uint16 major_image_version;
  uint16 minor_image_version;
  uint16 major_subsys_version;
  uint16 minor_subsys_version;
  uint32 win32_version_value;
  uint32 size_of_image;
  uint32 size_of_headers;
  uint32 checksum;
  uint16 subsystem;
  uint16 dll_characteristics;
  uint32 size_of_stack_reserve;
  uint32 size_of_stack_commit;
  uint32 size_of_heap_reserve;
  uint32 size_of_heap_commit;
  uint32 loader_flags;
  uint32 nrva;
  data_directory_t dd[16];
} __attribute__((packed)) pe_optional_header_t;

typedef struct
{
  uint8  name[8];
  uint32 physical_address;
  uint32 virtual_address;
  uint32 size_of_rawdata;
  uint32 pointer_to_rawdata;
  uint32 pointer_to_reloc;
  uint32 pointer_to_linenumbers;
  uint16 nreloc;
  uint16 nlinenumbers;
  uint32 characteristics;
} __attribute__((packed)) section_header_t;

typedef struct
{
  uint8* raw_file;

  dos_header_t*         dos_header;
  pe_header_t*          pe_header;
  coff_header_t*        coff_header;
  pe_optional_header_t* pe_optional_header;
  section_header_t*     section_headers;
} exe_t;

exe_t* parse_exe(const char* filename);
void   free_exe(exe_t*);

#endif
