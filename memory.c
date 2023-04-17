#include "header.h"
#include "mem.h"

static byte mem[MEMSIZE];

word reg[REGSIZE];    // reg[i] - это регистр Ri

void b_write(address adr, byte val)
{
	if (adr < 8)
	{
		if (val>>7 == 0)
			reg[adr] = (word)(val & OPCD_FOR_PL);
		else
			reg[adr] = val | OPCD_FOR_NEG;
		logger(MORE_DEBUG," ---b_write:reg:%o---\n", reg[adr]);
		return;
	}
	if(adr == odata)
		printf("%c", val);
	mem[adr] = val;
}

byte b_read(address adr)
{
	if (adr < 8)
		return (reg[adr] & 0xffff);
	return mem[adr];
}

void w_write(address adr, word val)
{
	if (adr < 8)
	{
		reg[adr] = val;
		return;
	}
	
	if(adr % 2 != 0)
	{
		logger(ERROR, "Попытка записать слово по нечетному адресу!\n");
		logger(DEBUG, "adr:%04hx, val:%04hx\n", adr, val);
		exit(1);
	}
	
	if(adr == odata)
		printf("%c", val);
	
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
		logger(MORE_DEBUG, "%04hx %04hx\n", adr, N);
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

void mem_clear()
{
	memset(mem,0,sizeof(mem)/sizeof(mem[0]));
	for(int i = 0; i < REGSIZE; i++)
		reg[i] = 0;
}

void set_ostat()
{
	// b_write(ostat, (1<<7));
	w_write(ostat, 0xFF);
}