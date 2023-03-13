#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include "header.h"

void test_mem()
{
	address a;
	byte b0, b1, b0res, b1res;
	word w, wres;

 // пишем байт, читаем байт
	log(MORE_DEBUG, "Пишем и читаем байт по четному адресу\n");
	a = 0;
	b0 = 0x12;
	b_write(a, b0);
	b0res = b_read(a);
	log(MORE_DEBUG, "a=%06o b0=%hhx b0res=%hhx\n", a, b0, b0res);
	assert(b0 == b0res);

	log(MORE_DEBUG, "Пишем и читаем байт по нечетному адресу\n");
	a = 1;
	b0 = 0x12;
	b_write(a, b0);
	b0res = b_read(a);
	log(MORE_DEBUG, "a=%06o b0=%hhx b0res=%hhx\n", a, b0, b0res);
	assert(b0 == b0res);
	
	

	// пишем слово, читаем слово
	log(MORE_DEBUG, "Пишем и читаем слово\n");
	a = 2;		// другой адрес
	w = 0x3456;
	w_write(a, w);
	wres = w_read(a);
	log(MORE_DEBUG, "a=%06o w=%04x wres=%04x\n", a, w, wres);
	assert(w == wres);


	// пишем 2 байта, читаем 1 слово
	log(MORE_DEBUG, "Пишем 2 байта, читаем слово\n");
	a = 4;		// другой адрес
	w = 0xa1b2;
	// little-endian, младшие разряды по меньшему адресу
	b0 = 0xb2;
	b1 = 0xa1;
	b_write(a, b0);
	b_write(a+1, b1);
	wres = w_read(a);
	log(MORE_DEBUG, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
	assert(w == wres);
	
	
	// пишем слово, читаем 2 байта
	log(MORE_DEBUG,"Пишем слово, читаем 2 байта\n");
	a = 6; // другой адрес
	w = 0xb2d8;
	// little-endian, младшие разряды по меньшему адресу
	b0 = 0xd8;
	b1 = 0xb2;
	w_write(a, w);
	b0res = b_read(a);
	b1res = b_read(a+1);
	log(MORE_DEBUG, "a = %06o w = %04x b0 = %02hhx b1 = %02hhx b0res = %02hhx b1res = %02hhx\n", a, w, b0, b1, b0res, b1res);
	assert(b0res == b0 && b1res == b1);
	
}

int main(int argc, char* argv[])
{
	test_mem();
	address a = 0x0040;
	int b = 30;
	introduction(argc, argv);
	mem_dump(a,b);
	
	return 0;
}

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
	//assert(sizeof(adr) == 2  && adr >= ADR_MIN && adr  <= ADR_MAX && Некорректное значение адреса); 
	return (word)mem[adr+1]<<8 | (word)mem[adr];
}

void load_data(FILE * fin)
{
	address adr, N;
	byte b0;
	while(2 == fscanf(fin,"%04hx %04hx",&adr, &N))
	{	
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

void log(int level, char* message,...)
{
	if(log_level >= level)
	{
		va_list ap;
		va_start(ap,message);
		vfprintf(stdout, message, ap);
		va_end(ap);
	}
}

int set_log_level(int level)
{
	int old_log_level = log_level;
	
	log_level = level;
	
	return old_log_level;
}

void introduction(int argc, char* argv[])
{
	if(argc >=2 && (strstr(argv[1], "-t") == argv[1]))
		load_file(argv[2]);
	else
	{
		printf("Т.к. название файла не было передано, либо было передано некорректно, программа будет считывать данные с командной строки\n");
		printf("Для прочтения данных из файла, запустите программу с дополнением в конце \"-t название_файла\"\n");
		load_data(stdin);
	}
}