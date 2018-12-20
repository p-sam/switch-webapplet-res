#include "web.h"
#include <string.h>

void WebPageArg(webpage_arg_t* arg_out, const char* url) {
	memset(arg_out, 0, sizeof(webpage_arg_t));
	arg_out->word0 = 0x3;
	arg_out->dword4 = 0x5;
	arg_out->word8 = 0xd;
	arg_out->wordA = 0x1;
	arg_out->dwordC = 0x0;
	arg_out->byte10 = 0x1;
	arg_out->bytes[0] = 0x12;
	arg_out->bytes[2] = 0x1;
	arg_out->bytes[8] = 0x1;
	arg_out->bytes[9] = 0x1;
	arg_out->bytes[12] = 0xc;
	strncpy(arg_out->url, url, sizeof(arg_out->url)-1);
}