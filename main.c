#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef word address;

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];

void b_write(address adr, byte val);

byte b_read(address adr);

void w_write(address adr, word val);

word w_read(address adr);

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
	return (word)mem[adr+1]<<8 | (word)mem[adr];
}
	
void test_mem()
{
	address a;
	byte b0, b1, bres;
	word w, wres;

 // пишем байт, читаем байт
    fprintf(stderr, "Пишем и читаем байт по четному адресу\n");
    a = 0;
    b0 = 0x12;
    b_write(a, b0);
    bres = b_read(a);
    fprintf(stderr, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
    assert(b0 == bres);

    fprintf(stderr, "Пишем и читаем байт по нечетному адресу\n");
    a = 1;
    b0 = 0x12;
    b_write(a, b0);
    bres = b_read(a);
    fprintf(stderr, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
    assert(b0 == bres);
	
	

    // пишем слово, читаем слово
    fprintf(stderr, "Пишем и читаем слово\n");
    a = 2;        // другой адрес
    w = 0x3456;
    w_write(a, w);
    wres = w_read(a);
    fprintf(stderr, "a=%06o w=%04x wres=%04x\n", a, w, wres);
    assert(w == wres);


    // пишем 2 байта, читаем 1 слово
    fprintf(stderr, "Пишем 2 байта, читаем слово\n");
    a = 4;        // другой адрес
    w = 0xa1b2;
    // little-endian, младшие разряды по меньшему адресу
    b0 = 0xb2;
    b1 = 0xa1;
    b_write(a, b0);
    b_write(a+1, b1);
    wres = w_read(a);
    fprintf(stderr, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
    assert(w == wres);
	
}

void load_data() //мб просто load_file сделать и при load_data stdin писать
{
	address adr, N;
	byte b0;
	while(2 == scanf("%04hx %04hx",&adr, &N))
    {    
        for(address i = 0; i < N; i++, adr++)
        {
            scanf("%04hhx", &b0);
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
	
int main(int argc, char* argv[])
{
    test_mem();
	address a = 0x0040;
	int b = 30;
	if(argc >=2)
		load_file(argv[1]);
	else
	{
		printf("Т.к. название файла не было передано, программа будет считывать данные с командной строки\n");
		printf("Для прочтения данных из файла, запустите программу с дополнением в конце \"-t название_файла\"\n");
		load_data();
	}
	mem_dump(a,b);
    return 0;
}
