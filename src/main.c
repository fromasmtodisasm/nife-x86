#include <stdio.h>
#include <string.h>

#include "exe.h"

int test_types()
{
  if(sizeof(uint8) != 8)
    return -1;

  if(sizeof(uint16) != 16)
    return -1;

  if(sizeof(uint32) != 32)
    return -1;

  if(sizeof(int8) != 8)
    return -1;

  if(sizeof(int16) != 16)
    return -1;

  if(sizeof(int32) != 32)
    return -1;

  return 0;
}

void usage(char** argv)
{
  printf("USAGE: %s [--usage|FILENAME]\n", argv[0]);
}

int handle(const char* arg)
{
  exe_t* exe = parse_exe(arg);

  if(!exe)
    return -1;

  return 0;
}

int main(int argc, char** argv)
{
  if(!test_types())
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
