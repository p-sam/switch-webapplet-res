#include "web_online.h"
#include <string.h>
#include <malloc.h>

Result webOnlineCreate(WebOnlineConfig* config, const char* url) {
	Result rc=0;
	memset(config, 0, sizeof(WebOnlineConfig));
	config->arg.word0 = 0x3;
	config->arg.dword4 = 0x5;
	config->arg.word8 = 0xd;
	config->arg.wordA = 0x1;
	config->arg.dwordC = 0x0;
	config->arg.byte10 = 0x1;
	config->arg.bytes[0] = 0x12;
	config->arg.bytes[2] = 0x1;
	config->arg.bytes[8] = 0x1;
	config->arg.bytes[9] = 0x1;
	config->arg.bytes[12] = 0xc;
	strncpy(config->arg.url, url, sizeof(config->arg.url)-1);
	
	config->workbuf_size = 0x1000;
	config->workbuf = (u8*)memalign(0x1000, config->workbuf_size);
	if (config->workbuf==NULL) rc = MAKERESULT(Module_Libnx, LibnxError_OutOfMemory);
	if (R_SUCCEEDED(rc)) memset(config->workbuf, 0, config->workbuf_size);

	return rc;
}


Result webOnlineShow(WebOnlineConfig* config) {
	Result rc = 0;
	AppletHolder holder;
	AppletStorage storage;

	memset(&storage, 0, sizeof(AppletStorage));

	rc = appletCreateLibraryApplet(&holder, AppletId_web, LibAppletMode_AllForeground);
	if (R_FAILED(rc)) return rc;

	LibAppletArgs commonargs;
	libappletArgsCreate(&commonargs, 0x50000);
	rc = libappletArgsPush(&commonargs, &holder);

	if (R_SUCCEEDED(rc)) rc = libappletPushInData(&holder, &config->arg, sizeof(config->arg));
	
	if (R_SUCCEEDED(rc)) {
		if (R_SUCCEEDED(rc)) rc = appletCreateTransferMemoryStorage(&storage, config->workbuf, config->workbuf_size, true);
		appletHolderPushInData(&holder, &storage);
	}

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