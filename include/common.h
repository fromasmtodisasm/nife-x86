#ifndef __COMMON_H__
#define __COMMON_H__

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;

typedef char  int8;
typedef short int16;
typedef long  int32;

#define DELETE(X) do { if(X) free(X); } while(0);

#endif
