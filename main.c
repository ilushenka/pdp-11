#include "header.h"
#include "run.h"
#include "tests.h"

int logger_level = TRACE;

int main(int argc, char* argv[])
{
	all_tests();
	set_ostat();
	introduction(argc, argv);
	run();
	address a = 0x0040;
	int b = 30;
	mem_dump(a,b);
	return 0;
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

void logger(int level, char* message,...)
{
	if(logger_level >= level)
	{
		va_list ap;
		va_start(ap,message);
		vfprintf(stdout, message, ap);
		va_end(ap);
	}
}

int set_logger_level(int level)
{
	int old_logger_level = logger_level;
	
	logger_level = level;
	
	return old_logger_level;
}