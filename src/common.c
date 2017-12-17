#include "common.h"

#include <stdio.h>

uint8* read_whole_file(const char* name)
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

