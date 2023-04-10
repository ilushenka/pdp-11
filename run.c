#include "run.h"

Command cmd[] = 
{
	{0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
	{0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
	{0177777, 0000000, "halt", do_halt, NO_PARAMS},
	{0177000, 0077000, "sob", do_sob, HAS_R | HAS_NN},
	{0170000, 0110000, "movb", do_movb, HAS_SS | HAS_DD},
	{0000000, 0000000, "unknown", do_nothing, NO_PARAMS} //Этот элемент массива оставлять в самом конце, т.к. он нужен для выхода из цикла
};

Argument ss, dd, reg_num;

int B_or_W = 1; //Если операция mov байтовая, то ноль, если со словом - один

void run()
{
	
	pc = 01000;
	
	Command oper;
	
	while(1)
	{
		oper = parse_cmd(read_cmd());	
		oper.do_command();
		logger(TRACE, "\n");
	}	
}

word read_cmd()
{	
	word w = w_read(pc);
		
	logger(TRACE, "%06o %06o:", pc, w);
	
	return w;
}

Command parse_cmd(word w)
{
	for(int i = 0;; i++)
		{
			if((w & cmd[i].mask) == cmd[i].opcode)
			{	
				pc += 2;
				logger(TRACE, "%s ", cmd[i].name);
				if(strcmp(cmd[i].name, "movb") == 0)
				{
					B_or_W = 0;
				}
				if((cmd[i].params & HAS_SS) == HAS_SS)
					ss = get_mr(w>>6);
				if((cmd[i].params & HAS_DD) == HAS_DD)	
					dd = get_mr(w);
				if(cmd[i].params  == (HAS_R | HAS_NN))					
					sob_init(w);
				
				logger(TRACE, "\n");
				return cmd[i];
			}
		}
}

void do_halt()
{
	logger(TRACE, "\n");
	
	reg_dump();
	
	logger(TRACE, "Завершение программы при помощи комманды halt\n");
	
	exit(0);
}

void do_mov()
{
	w_write(dd.adr, ss.val);
}

void do_movb()
{
	b_write(dd.adr, (byte)ss.val);
	logger(MORE_DEBUG, "---to_val:%o, to_adr:%o, from_val:%o, from_adr:%o---\n", b_read(dd.adr), dd.adr, ss.val, ss.adr);
}

void do_add()
{
	logger(MORE_DEBUG, "---BEFORE OPERATION ADD:dd.adr:%o, dd.val:%o, ss.val:%o, w.adr:%o, b.adr:%o---\n", dd.adr, dd.val, ss.val, w_read(dd.adr), b_read(dd.adr));
	w_write(dd.adr, (ss.val+dd.val));
	logger(MORE_DEBUG, "---AFTER OPERATION ADD:dd.adr:%o, dd.val:%o, ss.val:%o, w.adr:%o, b.adr:%o, reg:%o---\n", dd.adr, dd.val, ss.val, w_read(dd.adr), b_read(dd.adr), reg[dd.adr]);
}

void do_nothing()
{
}

void reg_dump()
{
	for(int i = 0; i < REGSIZE; i++)
		logger(TRACE, "R%d:%0o ", i, reg[i]);
	logger(TRACE, "\n");
}

Argument get_mr(word w)
{
	Argument res;
	int r = w & 7;      // номер регистра
	int m = (w>>3) & 7; // номер моды
	
	switch(m)
	{
	case 0:
		res.adr = r;
		res.val = reg[r];
		logger(TRACE, "R%d ", r);
		break;
	case 1:
		res.adr = reg[r];
		check_b_or_w_operation(&res);
		logger(TRACE, "(R%d) ", r);
		break;
	case 2:
		res.adr = reg[r];
		check_b_or_w_operation(&res);
		if(r < 6 && B_or_W == 0) 		//проверка какой именно регистр использован(т.к. для 6 и 7 регистров адреса должны быть четными, а при битовых операциях адрес может меняться на 1) 
			reg[r] += 1;
		else
			reg[r] += 2;
		if(r == 7)
            logger(TRACE, "#%o ", res.val);
        else
            logger(TRACE, "(R%d)+ ", r);
        break;
	case 3:
		res.adr = w_read(reg[r]);
		check_b_or_w_operation(&res);
		reg[r] += 2;
		if(r == 7)
			logger(TRACE, "@#%o ", res.val);
		else
			logger(TRACE, "@(R%d)+ ", r);
		break;
	case 4:
		if(r < 6 && B_or_W == 0) 		//проверка какой именно регистр использован(т.к. для 6 и 7 регистров адреса должны быть четными, а при битовых операциях адрес может меняться на 1) 
			reg[r] -= 1;
		else
			reg[r] -= 2;
		res.adr = reg[r];
		check_b_or_w_operation(&res);
		logger(TRACE,"-(R%d) ", r);
		break;
	case 5:
		if(r < 6 && B_or_W == 0) 		//проверка какой именно регистр использован(т.к. для 6 и 7 регистров адреса должны быть четными, а при битовых операциях адрес может меняться на 1) 
			reg[r] -= 1;
		else
			reg[r] -= 2;
		res.adr = w_read(reg[r]);
		check_b_or_w_operation(&res);
		logger(TRACE, "@-(R%d)+ ", r);
		break;
	default:
		logger(ERROR, "\n");
		logger(ERROR, "Мода %d еще не была реализована!\n", m);
		//exit(1);
	}
	return res;
}

void sob_init(word w)
{
	reg_num.adr = w>>6 & 7;
	reg_num.val = w & MAX_NN;
	logger(TRACE, "R%d, %o", reg_num.adr, pc - 2 * reg_num.val);
}


void do_sob()
{
	if(--reg[reg_num.adr] > 0)
		pc -= 2 * reg_num.val;
	else
		reg[reg_num.adr] = 0;
}

void check_b_or_w_operation(Argument * res)
{
	if(B_or_W)		//проверка побитовая операция или нет
	{	
		logger(MORE_DEBUG, "операция со словом\n");
		res->val = w_read(res->adr);
	}
	else
	{
		logger(MORE_DEBUG,"операция с байтом\n");
		res->val = b_read(res->adr);
	}
	logger(MORE_DEBUG, "---res_adr:%o, res->val:%o---\n", res->adr, res->val);
}