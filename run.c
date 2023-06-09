#include "run.h"

Command cmd[] = 
{
	{0170000, 0060000, "add", do_add, HAS_SS | HAS_DD},
	{0170000, 0010000, "mov", do_mov, HAS_SS | HAS_DD},
	{0177777, 0000000, "halt", do_halt, NO_PARAMS},
	{0177000, 0077000, "sob", do_sob, HAS_R | HAS_NN},
	{0170000, 0110000, "movb", do_movb, HAS_SS | HAS_DD},
	{0177400, 0000400, "br", do_br, HAS_XX},
	{0177400, 0103000, "bcc", do_bcc, HAS_XX},
	{0177400, 0103400, "bcs", do_bcs, HAS_XX},
	{0177400, 0001400, "beq", do_beq, HAS_XX},
	{0177400, 0100400, "bmi", do_bmi, HAS_XX},
	{0177400, 0001000, "bne", do_bne, HAS_XX},
	{0177400, 0100000, "bpl", do_bpl, HAS_XX},
	{0177700, 0005700, "tst", do_TST, HAS_DD},
	{0177700, 0105700, "tstb", do_TST, HAS_DD},
	{0170000, 0120000, "cmpb", do_CMP, HAS_SS | HAS_DD},
	{0170000, 0020000, "cmp", do_CMP, HAS_SS | HAS_DD},
	{0177000, 0005000, "clr", do_clr, HAS_DD},
	{0177000, 0105000, "clrb", do_clr, HAS_DD},
	{0177000, 0004000, "jsr", do_jsr, HAS_DD | HAS_R},
	{0177770, 0000200, "rts", do_rts, HAS_R},
	{0000000, 0000000, "unknown", do_nothing, NO_PARAMS} //Этот элемент массива оставлять в самом конце, т.к. он нужен для выхода из цикла
};

Argument ss, dd, reg_num;

int B_or_W = 1; //Если операция mov байтовая, то ноль, если со словом - один

int flag_N = 0, flag_Z = 0, flag_C = 0;

signed char XX = 0;

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
				
				if((w>>15) & 1)
					B_or_W = 0;
				else
					B_or_W = 1;
				
				if((cmd[i].params & HAS_SS) == HAS_SS)
					ss = get_mr(w>>6);
				if((cmd[i].params & HAS_DD) == HAS_DD)	
					dd = get_mr(w);
				
				if(cmd[i].params  == (HAS_R | HAS_NN))					
					sob_init(w);
				else
				if((cmd[i].params & HAS_R) == HAS_R && strcmp(cmd[i].name,"rts") == 0)
					get_r_value(w);
				else
				if((cmd[i].params & HAS_R) == HAS_R)
					get_r_value(w>>6);
				
				if(cmd[i].params == HAS_XX)
					get_XX(w);
				logger(TRACE, "\n");
				return cmd[i];
			}
		}
}

void reg_dump()
{
	for(int i = 0; i < REGSIZE-2; i++)
		logger(TRACE, "R%d:%06o ", i, reg[i]);
	logger(TRACE,"sp:%06o pc:%06o ", sp, pc);
	logger(TRACE, "Z:%d C:%d N:%d", flag_Z, flag_C, flag_N);
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
			logger(TRACE, "@#%o ", res.adr);
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
		reg[r] -= 2;
		res.adr = w_read(reg[r]);
		check_b_or_w_operation(&res);
		logger(TRACE, "@-(R%d)+ ", r);
		break;
	case 6:
		{
		word n = w_read(pc);
		pc += 2;
		res.adr = reg[r] + n;
		check_b_or_w_operation(&res);
		logger(TRACE, "%o(R%d) ", n, r);
		}
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

void check_NZ_flags(word res)
{
	flag_Z = res == 0;
	flag_N = (res>>(B_or_W ? 15 : 7)) & 1;
}

void check_C_flag(longword res)
{	
	flag_C = (res>>(B_or_W ? 16 : 8)) == 1;
}

void get_XX(word w)
{
	XX = w;
	logger(TRACE,"XX:%d to:%o ", XX, pc+2*XX);
}

void get_r_value(word w)
{
	reg_num.adr = w & 7;
	logger(TRACE, "R%d", reg_num.adr);
}