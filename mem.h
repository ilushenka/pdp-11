#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef word address;

#define REGSIZE 8
#define MEMSIZE (64*1024)
#define pc reg[7]
#define sp reg[6]
#define OPCD_FOR_PL 255
#define OPCD_FOR_NEG 255<<8
#define ostat 0177564
#define odata 0177566

extern word reg[REGSIZE];	// reg[i] - это регистр Ri

void b_write(address adr, byte val);

byte b_read(address adr);

void w_write(address adr, word val);

word w_read(address adr);

void load_data();

void mem_dump(address adr, int size);

void load_file(const char * filename);

void set_ostat();