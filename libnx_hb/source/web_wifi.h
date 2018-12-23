#pragma once
#include <switch.h>

typedef struct {
	u8 unk1[0x4];
	char url[0xFC];
} WebWifiPageArgUrl;

typedef struct {
	WebWifiPageArgUrl url1;
	WebWifiPageArgUrl url2;
	u8 unkgap1 [0x300];
	u8 unk2[0x18];
} WebWifiPageArg;

typedef struct {
	WebWifiPageArg arg;
} WebWifiConfig;

void webWifiCreate(WebWifiConfig* config, const char* url);
Result webWifiShow(WebWifiConfig* config);
