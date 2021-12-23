#define _GNU_SOURCE
#include <stdio.h>
#include "jasovervakning.h"
#include "config.h"

void debugLog(const char* fmt, ...) {
#ifdef DEBUGLOG
    char buffer[1024];
    // if I ever send debug string longer than 1024 bytes - "HELIHUD: ",
    // I will never find this error why application crashes :-)
    va_list ap;
    va_start(ap, fmt);
    strcpy(buffer, "jasovervakning:  ");
    vsprintf(buffer + 9, fmt, ap);
    va_end(ap);
    XPLMDebugString(buffer);
#endif
}

void infoLog(const char* fmt, ...) {
    char buffer[1024];
    // if I ever send debug string longer than 1024 bytes - "HELIHUD: ",
    // I will never find this error why application crashes :-)
    va_list ap;
    va_start(ap, fmt);
    strcpy(buffer, "jasovervakning:  ");
    vsprintf(buffer + 9, fmt, ap);
    va_end(ap);
    XPLMDebugString(buffer);
}