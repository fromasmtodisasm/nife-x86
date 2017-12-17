#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

exe_t* parse_exe(const char* name)
{
  exe_t* exe = malloc(sizeof(exe_t));

  exe->raw_file           = read_whole_file(name);
  exe->dos_header         = NULL;
  exe->pe_header          = NULL;
  exe->coff_header        = NULL;
  exe->pe_optional_header = NULL;
  exe->section_headers    = NULL;

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

  if((exe->coff_header->characteristics & IMAGE_FILE_EXECUTABLE_IMAGE) == IMAGE_FILE_EXECUTABLE_IMAGE)
  {
    printf("[Executable file]\n");
  }
  else
  {
    printf("[Objective file]\n");
  }

  printf("OS Version required: %d.%d\n", exe->pe_optional_header->major_osversion,
		                         exe->pe_optional_header->minor_osversion);

  printf("Program version: %d.%d\n", exe->pe_optional_header->major_image_version,
		                     exe->pe_optional_header->minor_image_version);

  printf("Subsystem: ");

  switch(exe->pe_optional_header->subsystem)
  {
    case IMAGE_SUBSYSTEM_NATIVE: printf("Native\n");               break;
    case IMAGE_SUBSYSTEM_WINGUI: printf("Windows GUI\n");          break;
    case IMAGE_SUBSYSTEM_WINCUI: printf("Windows Command-line\n"); break;
    default:                     printf("Unknown\n");              break;
  }

  int nsections = exe->coff_header->nsections;

  addr += sizeof(pe_optional_header_t);
  
  exe->section_headers = malloc(nsections * sizeof(section_header_t));
  memcpy(exe->section_headers, &exe->raw_file[addr], nsections * sizeof(section_header_t));

  int section, j;
  for(section = 0; section < nsections; section++)
  {
    printf("Section %d ", section);
    
    for(j = 0; j < 8; j++)
    {
      char c = exe->section_headers[section].name[j];

      if(c)
      {
        printf("%c", c);
      }
    }

    int begin = exe->section_headers[section].pointer_to_rawdata;
    int end   = begin + exe->section_headers[section].size_of_rawdata;

    printf(" %d %d\n", begin, end);
  }

  return exe;
}

void free_exe(exe_t* exe)
{
  free(exe->raw_file);

  DELETE(exe->dos_header);
  DELETE(exe->pe_header);
  DELETE(exe->coff_header);
  DELETE(exe->section_headers);

  free(exe);
}
