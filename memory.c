#include "header.h"

static byte mem[MEMSIZE];

void b_write(address adr, byte val)
{
	mem[adr] = val;
}

byte b_read(address adr)
{
	return mem[adr];
}

void w_write(address adr, word val)
{
	mem[adr+1] = val>>8;
	mem[adr] = val;
}

word w_read(address adr)
{
	assert(sizeof(adr) == 2  && adr >= ADR_MIN && adr  <= ADR_MAX && "Некорректное значение адреса"); 
	return (word)mem[adr+1]<<8 | (word)mem[adr];
}

void load_data(FILE * fin)
{
	address adr, N;
	byte b0;
	while(2 == fscanf(fin,"%04hx %04hx",&adr, &N))
	{	
		log(MORE_DEBUG, "%04hx %04hx", adr, N);
		for(address i = 0; i < N; i++, adr++)
		{
			fscanf(fin, "%04hhx", &b0);
			b_write(adr, b0);
		}
	}
}

void mem_dump(address adr, int size)
{
	for(int i = 0; i < size; i += 2)
	{
		printf("%06o: %06o %04x\n", adr+i, w_read(adr+i), w_read(adr+i));
	}
}

void load_file(const char * filename)
{
	FILE * fin = fopen(filename, "r");
	if (fin == NULL)
	{
		perror(filename);
		exit(errno);
	}
	load_data(fin);
}














