#pragma once
#include "header.h"
#include "mem.h"
#include "commands.h"

#define MAX_NN 63

typedef struct
{
	address adr;
	word val;
} Argument;

extern Argument ss, dd, reg_num;
extern int flag_Z, flag_C, flag_N;
extern signed char XX;

void run();

void reg_dump();

void sob_init(word w);

Argument get_mr(word w);

Command parse_cmd(word w);

word read_cmd();

void check_b_or_w_operation(Argument * res);

void check_NZ_flags(word res);

void check_C_flag(word a1, word a2);

void get_XX(word w);