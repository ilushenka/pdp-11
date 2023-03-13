#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef word address;

#define MEMSIZE (64*1024)
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

void b_write(address adr, byte val);

byte b_read(address adr);

void w_write(address adr, word val);

word w_read(address adr);

void load_data();

void mem_dump(address adr, int size);

void load_file(const char * filename);

void log(int level, char* message,...);

int set_log_level(int level);

void introduction(int argc, char* argv[]);
