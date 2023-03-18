#pragma once
#include "header.h"

#define MAX_NUM_FUNC 3

void run();

void do_halt();

void do_add();

void do_mov();

void do_nothing();

typedef struct
{
	word mask;
	word opcode;
	char * name;
	void (*do_command)(void);
} Command;
