#include "web_wifi.h"
#include <string.h>
#include <malloc.h>

static void _webWifiUrlCreate(WebWifiPageArgUrl* argUrl, const char* url) {
	strncpy(argUrl->url, url, sizeof(argUrl->url)-1);
}

void webWifiCreate(WebWifiConfig* config, const char* url) {
	memset(config, 0, sizeof(WebWifiConfig));
	_webWifiUrlCreate(&config->arg.url1, url);
	_webWifiUrlCreate(&config->arg.url2, url);
}


Result webWifiShow(WebWifiConfig* config) {
	Result rc = 0;
	AppletHolder holder;
	AppletStorage storage;

	memset(&storage, 0, sizeof(AppletStorage));

	rc = appletCreateLibraryApplet(&holder, AppletId_wifiWebAuth, LibAppletMode_AllForeground);
	if (R_FAILED(rc)) return rc;

	LibAppletArgs commonargs;
	libappletArgsCreate(&commonargs, 0);
	rc = libappletArgsPush(&commonargs, &holder);

	if (R_SUCCEEDED(rc)) rc = libappletPushInData(&holder, &config->arg, sizeof(config->arg));

	if (R_SUCCEEDED(rc)) rc = appletHolderStart(&holder);
	
	if (R_SUCCEEDED(rc)) {
		while(appletHolderWaitInteractiveOut(&holder)) {
			//TODO: Handle Interactive data here.
		}
	}

	if (R_SUCCEEDED(rc)) {
		appletHolderJoin(&holder);

		LibAppletExitReason reason = appletHolderGetExitReason(&holder);

		if (reason == LibAppletExitReason_Abnormal || reason == LibAppletExitReason_Unexpected) {
			//TODO: Official sw asserts here - return a proper error here.
			return -1;
		}
	}

	appletHolderClose(&holder);

	return rc;
}