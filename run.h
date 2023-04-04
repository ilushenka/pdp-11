#include "header.h"
#define MAX_NN 63

enum
{
	NO_PARAMS,
	HAS_DD,
	HAS_SS,
	HAS_R = 4,
	HAS_NN = 8
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

void do_nothing();

void reg_dump();

void sob_init(word w);

void do_sob();

Argument get_mr(word w);

Command parse_cmd(word w);

word read_cmd();