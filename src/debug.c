/*
 * Filename: debug.c
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Utilises <3dslink.h> for
 * debugging capabilities.
 */


#include "debug.h"


static u32 *SOC_buffer = NULL;
s32 sock = -1, csock = -1;


int Debug_Init(void)
{
	int ret;
	// start 3dslink with -s to see printf output on host

	// allocate buffer for SOC service
	SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

	if(SOC_buffer == NULL) {
		Debug_FailExit("memalign: failed to allocate\n");
	}

	// Now intialise soc:u service
	if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
    	return EXIT_FAILURE;
	}

	// register socShutdown to run at exit
	atexit(Debug_SocShutdown);

	link3dsStdio();

    return EXIT_SUCCESS;
}


void Debug_SocShutdown(void)
{
	if(sock>0) close(sock);
	if(csock>0) close(csock);
	printf("waiting for Debug_SocExit...\n");
	socExit();
}


void Debug_FailExit(const char *fmt, ...)
{
	if(sock>0) close(sock);
	if(csock>0) close(csock);
}