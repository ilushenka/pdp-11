#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#define ADR_MIN 0
#define ADR_MAX (1024*64)

enum loggernum
{
	ERROR,
	INFO,
	TRACE,
	DEBUG,
	MORE_DEBUG
};

void logger(int level, char* message,...);

int set_logger_level(int level);

void introduction(int argc, char* argv[]);
