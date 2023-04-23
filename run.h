#pragma once
#include "header.h"
#include "mem.h"
#include "commands.h"

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
	address adr;
	word val;
} Argument;

typedef unsigned int longword;

extern Argument ss, dd, reg_num;
extern int flag_Z, flag_C, flag_N, B_or_W;
extern signed char XX;

void run();

void reg_dump();

void sob_init(word w);

Argument get_mr(word w);

Command parse_cmd(word w);

word read_cmd();

void check_b_or_w_operation(Argument * res);

void check_NZ_flags(word res);

void check_C_flag(longword res);

void get_XX(word w);

void get_r_value(word w);