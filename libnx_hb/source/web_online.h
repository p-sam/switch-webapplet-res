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
	char gap;
	char url[0xC00];
	char padding[0x13DB];
} WebOnlinePageArg;

typedef struct {
	WebOnlinePageArg arg;
	u8* workbuf;
	size_t workbuf_size;
} WebOnlineConfig;

Result webOnlineCreate(WebOnlineConfig* config, const char* url);
Result webOnlineShow(WebOnlineConfig* config);
