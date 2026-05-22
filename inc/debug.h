/*
 * Filename: debug.h
 * Author:   AllAcacia
 * 
 * MODULE INFO:
 * Utilises <3dslink.h> for
 * debugging capabilities.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <3ds.h>

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000


int Debug_Init(void);

void Debug_SocShutdown(void);

void Debug_FailExit(const char *fmt, ...);

#define DEBUG_MODE_ALLOWED // used to determine if debug print functions compules in full, else returns blankly.

#endif // DEBUG_H