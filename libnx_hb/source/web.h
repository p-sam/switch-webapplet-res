#pragma once
#include <switch.h>

typedef struct {
  u16 word0;
  char gap1[2];
  u32 dword4;
  u16 word8;
  u16 wordA;
  u32 dwordC;
  char byte10;
  char bytes[0x11];
  char url[0xC00];
  char padding[0x13DB];
} webpage_arg_t;

static_assert(sizeof(webpage_arg_t) == 0x2000, "struct wrong");

void WebPageArg(webpage_arg_t* arg_out, const char* url);