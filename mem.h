#pragma once
typedef unsigned char byte;
typedef unsigned short word;
typedef word address;

#define REGSIZE 8
#define MEMSIZE (64*1024)
#define pc reg[7]

word reg[REGSIZE];    // reg[i] - это регистр Ri

void b_write(address adr, byte val);

byte b_read(address adr);

void w_write(address adr, word val);

word w_read(address adr);