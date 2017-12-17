#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;

#define DELETE(X) do { if(X) free(X); } while(0);

uint8* read_whole_file(const char* name);

#endif
