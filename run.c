#include "run.h"

Command cmd[] = 
{
	{0170000, 0060000, "add\n", do_add},
	{0170000, 0010000, "mov\n", do_mov},
	{0177777, 0000000, "halt\n", do_halt}
};

void run()
{
	
	pc = 01000;
	
	word w;
	
	while(1)
	{
		w = w_read(pc);
		
		log(TRACE, "%06o %06o:", pc, w);
		
		for(int i = 0; i < MAX_NUM_FUNC; i++)
		{
			if((w & cmd[i].mask) == cmd[i].opcode)
			{
				log(TRACE, cmd[i].name);
				cmd[i].do_command();
				break;
			}
			else
			if(i == MAX_NUM_FUNC - 1)
			{
				log(TRACE, "unknown\n");
				do_nothing();
			}
		}
		pc += 2;
	}	
}

void do_halt()
{
	log(TRACE, "Завершение программы\n");
	exit(0);
}

void do_add()
{
}

void do_mov()
{
}

void do_nothing()
{
}