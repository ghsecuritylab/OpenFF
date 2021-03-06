
#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

char hostname[128];
#define LOG(level)	if (level <= logLevel) log_print
void log_open(const char *file, int level);
void log_close();
void log_print(const char *fmt, ...);
int logLevel;
#endif
