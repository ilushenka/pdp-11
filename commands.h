#pragma once
#include "header.h"
typedef struct
{
	word mask;
	word opcode;
	char * name;
	void (*do_command)(void);
	char params;
} Command;

void do_halt();

void do_add();

void do_mov();

void do_movb();

void do_nothing();

void do_clr();

void do_br();

void do_bcc();

void do_bcs();

void do_beq();

void do_bne();

void do_bmi();

void do_bpl();

void do_TST();

void do_CMP();

void do_sob();

void do_jsr();

void do_rts();

