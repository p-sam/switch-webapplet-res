#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <switch.h>
#include "web_online.h"
#include "web_wifi.h"

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

void showKeyboard() {
	printf("* showKeyboard\n");
	Result rc = 0;
	SwkbdConfig config;
	swkbdCreate(&config, 0);
	char resultStr[500] = {0};
	rc = swkbdShow(&config, resultStr, sizeof(resultStr));
	PRINT_RC("swkbdShow", rc);
	
	if(R_SUCCEEDED(rc)) {
		printf("-> %s\n", resultStr);
	}
}

void showWebOnline() {
	printf("* showWebOnline\n");
	Result rc = 0;
	WebOnlineConfig config;
	printf("sizeof(config): %lx\n", sizeof(config));
	printf("sizeof(config.arg): %lx\n", sizeof(config.arg));
	printf("sizeof(config.workbuf): %lx\n", sizeof(config.workbuf));
	
	rc = webOnlineCreate(&config, START_URL);
	PRINT_RC("webOnlineCreate", rc);
	rc = webOnlineShow(&config);
	PRINT_RC("webOnlineShow", rc);
}


void showWebWifi() {
	printf("* showWebWifi\n");
	Result rc = 0;
	WebWifiConfig config;
	printf("sizeof(config): %lx\n", sizeof(config));
	printf("sizeof(config.arg): %lx\n", sizeof(config.arg));
	
	webWifiCreate(&config, START_URL);
	rc = webWifiShow(&config);
	PRINT_RC("webWifiShow", rc);
}

int main(int argc, char *argv[]) {
	printf(
		"START\n"
		"======\n"
		"Press X to show keyboard\n"
		"Press Y to show a webpage using web\n"
		"Press B to show a webpage using wifiwebauth\n"
		"======\n"
		"Press + to exit\n"
	);

	while (appletMainLoop()) {
		hidScanInput();

		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		
		if (kDown & KEY_PLUS) {
			break;
		}
		
		if (kDown & KEY_X) {
			showKeyboard();
		}

		if (kDown & KEY_Y) {
			showWebOnline();
		}

		if (kDown & KEY_B) {
			showWebWifi();
		}
		
		consoleUpdate(NULL);
	}

	return 0;
}