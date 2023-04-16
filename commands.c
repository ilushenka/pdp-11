#include "run.h"
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
	check_NZ_flags(ss.val);
}

void do_movb()
{
	b_write(dd.adr, (byte)ss.val);
	check_NZ_flags(ss.val);
	logger(MORE_DEBUG, "---to_val:%o, to_adr:%o, from_val:%o, from_adr:%o---\n", b_read(dd.adr), dd.adr, ss.val, ss.adr);
}

void do_add()
{
	logger(MORE_DEBUG, "---BEFORE OPERATION ADD:dd.adr:%o, dd.val:%o, ss.val:%o, w.adr:%o, b.adr:%o---\n", dd.adr, dd.val, ss.val, w_read(dd.adr), b_read(dd.adr));
	w_write(dd.adr, (ss.val+dd.val));
	check_NZ_flags((ss.val+dd.val));
	check_C_flag(ss.val, dd.val);
	logger(MORE_DEBUG, "---AFTER OPERATION ADD:dd.adr:%o, dd.val:%o, ss.val:%o, w.adr:%o, b.adr:%o, reg:%o---\n", dd.adr, dd.val, ss.val, w_read(dd.adr), b_read(dd.adr), reg[dd.adr]);
}

void do_sob()
{
	if(--reg[reg_num.adr] > 0)
		pc -= 2 * reg_num.val;
	else
		reg[reg_num.adr] = 0;
}

void do_nothing()
{
}

void do_TST(word d)
{
	check_NZ_flags(d);
	flag_C = 0;
}

void do_TSTb(byte d)
{
	do_TST((word)(d)<<8);
}

void do_CMP(word s, word d)
{
	check_NZ_flags((s-d));
	check_C_flag(s,(-d));
}

void do_CMPb(byte s, byte d)
{
	do_CMP(((word)(s)<<8),((word)(d)<<8)); 
}

void do_br()
{
	pc += 2*XX;
}

void do_bcc() // if Carry clear
{
	if(flag_C == 0)
		do_br();
}

void do_bcs() // if Carry set
{
	if(flag_C == 1)
		do_br();
}

void do_beq() // if equal
{
	if(flag_Z == 1)
		do_br();
}

void do_bne() // if not equal
{
	if(flag_Z == 0)
		do_br();
}

void do_bmi() //if negative
{
	if(flag_N == 1)
		do_br();
}

void do_bpl() // if plus
{
	if(flag_N == 0)
		do_br();
}