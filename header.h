#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include "mem.h"

#define ADR_MIN 0
#define ADR_MAX 65535

enum lognum
{
	ERROR,
	INFO,
	TRACE,
	DEBUG,
	MORE_DEBUG
};

void load_data();

void mem_dump(address adr, int size);

void load_file(const char * filename);

void log(int level, char* message,...);

int set_log_level(int level);

void introduction(int argc, char* argv[]);
