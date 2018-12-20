#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <switch.h>
#include "web.h"

#define PRINT_RC(str, rc) printf("[!] %s: [0x%x] %04d-%04d\n", str, rc, R_MODULE(rc), R_DESCRIPTION(rc))
#define START_URL "http://google.fr"

void userAppInit() {
	consoleInit(NULL);
	socketInitializeDefault();
	nxlinkStdio();
}

void userAppExit() {
	consoleExit(NULL);
	socketExit();
}

void job() {
	AppletHolder appletHolder = {0};
	Result rc = 0;
	
	rc = appletCreateLibraryApplet(&appletHolder, AppletId_web, LibAppletMode_AllForeground);
	PRINT_RC("appletCreateLibraryApplet", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	AppletStorage commonArgsStorage = {0};
	rc = appletCreateStorage(&commonArgsStorage, 0x20);
	PRINT_RC("appletCreateStorage[commonArgsStorage]", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	u32 commonArgsHeader[] = {0x1, 0x2000}; // {version?, size?}
	rc = appletStorageWrite(&commonArgsStorage, 0, commonArgsHeader, sizeof(commonArgsHeader));
	PRINT_RC("appletStorageWrite[commonArgsHeader]", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	struct {
		u32 unknown1;
		u32 theme_color;
		u8 unknown2;
		u64 system_tick;
	} commonArgsData = {0};
	static_assert(sizeof(commonArgsData) == 0x18, "invalid common args data");
	
	commonArgsData.unknown1 = 0x30000;
	commonArgsData.theme_color = 0;
	commonArgsData.unknown2 = 0;
	commonArgsData.system_tick = svcGetSystemTick();
	
	rc = appletStorageWrite(&commonArgsStorage, sizeof(commonArgsHeader), &commonArgsData, sizeof(commonArgsData));
	PRINT_RC("appletStorageWrite[commonArgsStorage]", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	rc = appletHolderPushInData(&appletHolder, &commonArgsStorage);
	PRINT_RC("appletHolderPushInData[commonArgsStorage]", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	webpage_arg_t webpageConfig = {0};
	WebPageArg(&webpageConfig, START_URL);
	
	AppletStorage configStorage = {0};
	rc = appletCreateStorage(&configStorage, 0x2000);
	PRINT_RC("appletCreateStorage[configStorage]", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	static_assert(sizeof(webpageConfig) == 0x2000, "invalid webpage_arg size");
	rc = appletStorageWrite(&configStorage, 0, &webpageConfig, sizeof(webpageConfig));
	PRINT_RC("appletStorageWrite[webpageConfig]", rc);
	
	if(R_FAILED(rc)) {
		return;
	}
	
	rc = appletHolderPushInData(&appletHolder, &configStorage);
	PRINT_RC("appletHolderPushInData[configStorage]", rc);
	if(R_FAILED(rc)) {
		return;
	}
	
	rc = appletHolderStart(&appletHolder);
	PRINT_RC("appletHolderStart", rc);
	if(R_FAILED(rc)) {
		return;
	}
}


int main(int argc, char *argv[]) {
	printf("START\n======\n");
	job();
	printf("\n======\nPress + to exit\n");

	while (appletMainLoop()) {
		hidScanInput();

		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		
		if (kDown & KEY_PLUS) {
			break;
		}
		
		consoleUpdate(NULL);
	}

	return 0;
}