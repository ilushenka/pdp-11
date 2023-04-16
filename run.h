#pragma once
#include "header.h"
#include "mem.h"

#define MAX_NN 63

enum
{
	NO_PARAMS,
	HAS_DD,
	HAS_SS,
	HAS_R = 1<<2,
	HAS_NN = 1<<3,
	HAS_XX = 1<<4
}parameters;

typedef struct
{
	word mask;
	word opcode;
	char * name;
	void (*do_command)(void);
	char params;
} Command;

typedef struct
{
	address adr;
	word val;
} Argument;

extern Argument ss, dd;

void run();

void do_halt();

void do_add();

void do_mov();

void do_movb();

void do_nothing();

void do_br();

void do_bcc();

void do_bcs();

void do_beq();

void do_bne();

void do_bmi();

void do_bpl();

void do_TST(word d);

void do_TSTb(byte d);

void do_CMP(word s, word d);

void do_CMPb(byte s, byte d);

void reg_dump();

void sob_init(word w);

void do_sob();

Argument get_mr(word w);

Command parse_cmd(word w);

word read_cmd();

void check_b_or_w_operation(Argument * res);

void check_NZ_flags(word res);

void check_C_flag(word a1, word a2);

void get_XX(word w);