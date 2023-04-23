#include "header.h"
#include "tests.h"
#include "run.h"

void all_tests()
{
	test_mem();
	test_ident_mov();
	test_ident_add();
	test_ident_halt();
	test_mode0();
	test_mov();
	test_add();
	test_mode1_toreg();
	test_mode1_fromreg();
	test_mode1_fromregtoreg();
	test_mode2_toreg();
	test_mode2_fromreg();
	test_mode2_fromregtoreg();
	test_mode3_toreg();
	test_mode3_fromreg();
	test_mode3_fromregtoreg();
	test_mode4_toreg();
	test_mode4_fromreg();
	test_mode4_fromregtoreg();
	test_mode5_toreg();
	test_mode5_fromreg();
	test_mode5_fromregtoreg();
	test_mode6_toreg();
	test_mode6_fromreg();
	test_mode6_fromregtoreg();

	test_mode2_workwithsp();
	test_mode4_workwithsp();
	
	mem_clear();
}

void test_mem()
{
	address a;
	byte b0, b1, b0res, b1res;
	word w, wres;
 // пишем байт, читаем байт
	logger(TRACE, "Пишем и читаем байт по четному адресу\n");
	a = 10;
	b0 = 0x12;
	b_write(a, b0);
	b0res = b_read(a);
	logger(TRACE, "a=%06o b0=%hhx b0res=%hhx\n", a, b0, b0res);
	assert(b0 == b0res);

	logger(TRACE, "Пишем и читаем байт по нечетному адресу\n");
	a = 11;
	b0 = 0x12;
	b_write(a, b0);
	b0res = b_read(a);
	logger(TRACE, "a=%06o b0=%hhx b0res=%hhx\n", a, b0, b0res);
	assert(b0 == b0res);
	
	

	// пишем слово, читаем слово
	logger(TRACE, "Пишем и читаем слово\n");
	a = 12;		// другой адрес
	w = 0x3456;
	w_write(a, w);
	wres = w_read(a);
	logger(TRACE, "a=%06o w=%04x wres=%04x\n", a, w, wres);
	assert(w == wres);


	// пишем 2 байта, читаем 1 слово
	logger(TRACE, "Пишем 2 байта, читаем слово\n");
	a = 14;		// другой адрес
	w = 0xa1b2;
	// little-endian, младшие разряды по меньшему адресу
	b0 = 0xb2;
	b1 = 0xa1;
	b_write(a, b0);
	b_write(a+1, b1);
	wres = w_read(a);
	logger(TRACE, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
	assert(w == wres);
	
	
	// пишем слово, читаем 2 байта
	logger(TRACE,"Пишем слово, читаем 2 байта\n");
	a = 16; // другой адрес
	w = 0xb2d8;
	// little-endian, младшие разряды по меньшему адресу
	b0 = 0xd8;
	b1 = 0xb2;
	w_write(a, w);
	b0res = b_read(a);
	b1res = b_read(a+1);
	logger(TRACE, "a = %06o w = %04x b0 = %02hhx b1 = %02hhx b0res = %02hhx b1res = %02hhx\n", a, w, b0, b1, b0res, b1res);
	assert(b0res == b0 && b1res == b1);
	
	//проверка на halt
	/*logger(TRACE, "Проверка на работу halt");
	run()
	*/
	
}

// тест, что мы правильно определяем команды mov, add, halt
void test_ident_mov()
{
	logger(TRACE, "Проверка определения команды mov\n");
	Command test_cmd = parse_cmd(0017654);
	assert(!strcmp(test_cmd.name, "mov"));
}

void test_ident_add()
{
	logger(TRACE, "Проверка определения команды add\n");
	Command test_cmd = parse_cmd(0067654);
	assert(!strcmp(test_cmd.name, "add"));
}

void test_ident_halt()
{
	logger(TRACE, "Проверка определения команды halt\n");
	Command test_cmd = parse_cmd(0000000);
	assert(!strcmp(test_cmd.name, "halt"));
}

// тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3
void test_mode0()
{
	logger(TRACE, "Тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3\n");
	reg[3] = 12;	// dd
	reg[5] = 34;	// ss
	Command test_cmd = parse_cmd(0010503);
	logger(TRACE, "ss.val:%d, ss.adr:%d, dd.val:%d, dd.adr:%d\n", ss.val, ss.adr, dd.val, dd.adr);
	assert(ss.val == 34);
	assert(ss.adr == 5);
	assert(dd.val == 12);
	assert(dd.adr == 3);
}
// тест, что mov и мода 0 работают верно в mov R5, R3
void test_mov()
{
	logger(TRACE, "Тест, что mov и мода 0 работают верно в mov R5, R3\n");
	reg[3] = 12;	// dd
	reg[5] = 34;	// ss
	Command test_cmd = parse_cmd(0010503);
	test_cmd.do_command();
	assert(reg[3] == 34);
	assert(reg[5] == 34);
}

void test_add()
{
	logger(TRACE, "Тест, что add и мода 0 работают верно в mov R5, R3\n");
	reg[3] = 12;	// dd
	reg[5] = 34;	// ss
	Command test_cmd = parse_cmd(0060503);
	test_cmd.do_command();
	logger(TRACE, "reg[3]:%d, reg[5]:%d\n", reg[3], reg[5]);
	assert(reg[3] == 46);
	assert(reg[5] == 34);
}

// тест, что мы разобрали правильно аргументы ss и dd в mov (R5), R3
void test_mode1_toreg()
{
	logger(TRACE, "Тест моды 1 вида mov (R5), R3\n");
// setup
	reg[3] = 12;	// dd
	reg[5] = 0200;  // ss
	w_write(0200, 34);

	Command test_cmd = parse_cmd(0011503);

	assert(ss.val == 34);
	assert(ss.adr == 0200);
	assert(dd.val == 12);
	assert(dd.adr == 3);

	test_cmd.do_command();

	assert(reg[3] == 34);
	// проверяем, что значение регистра не изменилось
	assert(reg[5] == 0200);
}

void test_mode1_fromreg()
{
	logger(TRACE, "Тест моды 1 вида mov R5, (R3)\n");
   
   // setup
	reg[5] = 12;  // ss
	reg[3] = 0202;	// dd
	w_write(0202, 34);

	Command test_cmd = parse_cmd(0010513);

	assert(dd.val == 34);
	assert(dd.adr == 0202);
	assert(ss.val == 12);
	assert(ss.adr == 5);

	test_cmd.do_command();

	assert(reg[5] == 12);
	assert(w_read(0202) == 12); //проверяем прошло ли успешно копирование
	// проверяем, что значение регистра не изменилось
	assert(reg[3] == 0202);

}

void test_mode1_fromregtoreg()
{
	logger(TRACE, "Тест моды 1 вида mov (R5), (R3)\n");
   
   // setup
	reg[5] = 0204;  // ss
	reg[3] = 0206;	// dd
	w_write(0204, 34);
	w_write(0206, 12);

	Command test_cmd = parse_cmd(0011513);

	assert(dd.val == 12);
	assert(dd.adr == 0206);
	assert(ss.val == 34);
	assert(ss.adr == 0204);

	test_cmd.do_command();

	assert(w_read(0204) == 34); //проверяем прошло ли успешно копирование
	assert(w_read(0206) == 34);

	// проверяем, что значение регистра не изменилось
	assert(reg[3] == 0206);
	assert(reg[5] == 0204);
}

// тест, что мы разобрали правильно аргументы ss и dd в mov (R5), R3
void test_mode2_toreg()
{
	logger(TRACE, "Тест моды 2 вида mov (R5)+, R3\n");
   
   // setup
	reg[3] = 12;	// dd
	reg[5] = 0200;  // ss
	w_write(0200, 34);

	Command test_cmd = parse_cmd(0012503);

	assert(ss.val == 34);
	assert(ss.adr == 0200);
	assert(dd.val == 12);
	assert(dd.adr == 3);

	test_cmd.do_command();

	assert(reg[3] == 34);
	// проверяем, что значение регистра увеличилось на 2
	assert(reg[5] == 0202);
}

void test_mode2_fromreg()
{
	logger(TRACE, "Тест моды 2 вида mov R5, (R3)+\n");
   
   // setup
	reg[5] = 12;  // ss
	reg[3] = 0202;	// dd
	w_write(0202, 34);

	Command test_cmd = parse_cmd(0010523);

	assert(dd.val == 34);
	assert(dd.adr == 0202);
	assert(ss.val == 12);
	assert(ss.adr == 5);

	test_cmd.do_command();

	assert(reg[5] == 12);
	assert(w_read(0202) == 12); //проверяем прошло ли успешно копирование
	// проверяем, что значение регистра увеличилось на 2
	assert(reg[3] == 0204);
}

void test_mode2_fromregtoreg()
{
	logger(TRACE, "Тест моды 2 вида mov (R5)+, (R3)+\n");
   
   // setup
	reg[5] = 0204;  // ss
	reg[3] = 0206;	// dd
	w_write(0204, 34);
	w_write(0206, 12);

	Command test_cmd = parse_cmd(0012523);

	assert(dd.val == 12);
	assert(dd.adr == 0206);
	assert(ss.val == 34);
	assert(ss.adr == 0204);

	test_cmd.do_command();

	assert(w_read(0204) == 34); //проверяем прошло ли успешно копирование
	assert(w_read(0206) == 34);

	// проверяем, что значение регистра увеличилось на 2
	assert(reg[3] == 0210);
	assert(reg[5] == 0206);

}

void test_mode2_workwithsp()
{
	logger(TRACE, "Тест на работу 2 моды с 6 регистром(sp) вида movb (R6)+ (R2)+\n");
	
	//setup
	reg[6] = 0340;  // ss
	reg[2] = 0344;	// dd
	b_write(0340, 34);
	b_write(0344, 12);

	Command test_cmd = parse_cmd(0112622);

	assert(dd.val == 12);
	assert(dd.adr == 0344);
	assert(ss.val == 34);
	assert(ss.adr == 0340);

	test_cmd.do_command();

	assert(b_read(0344) == 34); //проверяем прошло ли успешно копирование

	// проверяем, что значение регистра 6 увеличилось на 2, а регистра 2 - на 1
	assert(reg[2] == 0345);
	assert(reg[6] == 0342);
}

void test_mode3_toreg()
{
	logger(TRACE, "Тест моды 3 вида mov @(R5)+, R3\n");
   
   // setup
	reg[3] = 12;	// dd
	reg[5] = 0212;  // ss
	w_write(0212, 0216);
	w_write(0216, 34);

	Command test_cmd = parse_cmd(0013503);

	assert(ss.val == 34);
	assert(ss.adr == 0216);
	assert(dd.val == 12);
	assert(dd.adr == 3);

	test_cmd.do_command();

	assert(reg[3] == 34);
	// проверяем, что значение регистра увеличилось на 2
	assert(reg[5] == 0214);
}

void test_mode3_fromreg()
{
	logger(TRACE, "Тест моды 3 вида mov R5, @(R3)+\n");
   
   // setup
	reg[5] = 12;  // ss
	reg[3] = 0214;	// dd
	w_write(0214, 0220);
	w_write(0220, 34);

	Command test_cmd = parse_cmd(0010533);

	assert(dd.val == 34);
	assert(dd.adr == 0220);
	assert(ss.val == 12);
	assert(ss.adr == 5);

	test_cmd.do_command();

	assert(reg[5] == 12);
	assert(w_read(0220) == 12); //проверяем прошло ли успешно копирование
	// проверяем, что значение регистра увеличилось на 2
	assert(reg[3] == 0216);
}

void test_mode3_fromregtoreg()
{
	logger(TRACE, "Тест моды 3 вида mov @(R5)+, @(R3)+\n");
   
   // setup
	reg[5] = 0222;  // ss
	reg[3] = 0226;	// dd
	w_write(0222, 0224);
	w_write(0224, 34);
	w_write(0226, 0230);
	w_write(0230, 12);

	Command test_cmd = parse_cmd(0013533);

	assert(dd.val == 12);
	assert(dd.adr == 0230);
	assert(ss.val == 34);
	assert(ss.adr == 0224);

	test_cmd.do_command();

	assert(w_read(0224) == 34); //проверяем прошло ли успешно копирование
	assert(w_read(0230) == 34);

	// проверяем, что значение регистра увеличилось на 2
	assert(reg[3] == 0230);
	assert(reg[5] == 0224);
}

void test_mode4_toreg()
{
	logger(TRACE, "Тест моды 4 вида mov R5, -(R3)\n");
   
   // setup
	reg[5] = 34;  // ss
	reg[3] = 0244;	// dd
	w_write(0242, 12);

	Command test_cmd = parse_cmd(0010543);

	assert(dd.val == 12);
	assert(dd.adr == 0242);
	assert(ss.val == 34);
	assert(ss.adr == 5);

	test_cmd.do_command();

	assert(w_read(0242) == 34); //проверяем прошло ли успешно копирование

	assert(reg[3] == 0242);
	assert(reg[5] == 34);

}

void test_mode4_fromreg()
{
	logger(TRACE, "Тест моды 4 вида mov -(R5), R3\n");
   
   // setup
	reg[5] = 0250;  // ss
	reg[3] = 12;	// dd
	w_write(0246, 34);

	Command test_cmd = parse_cmd(0014503);

	assert(dd.val == 12);
	assert(dd.adr == 3);
	assert(ss.val == 34);
	assert(ss.adr == 0246);

	test_cmd.do_command();

	assert(reg[5] == 0246);
	assert(reg[3] == 34);
}

void test_mode4_fromregtoreg()
{
	logger(TRACE, "Тест моды 4 вида mov -(R5), -(R3)\n");
   
	// setup
	reg[5] = 0234;  // ss
	reg[3] = 0240;	// dd
	w_write(0232, 34);
	w_write(0236, 12);

	Command test_cmd = parse_cmd(0014543);

	assert(dd.val == 12);
	assert(dd.adr == 0236);
	assert(ss.val == 34);
	assert(ss.adr == 0232);

	test_cmd.do_command();

	assert(w_read(0232) == 34); //проверяем прошло ли успешно копирование
	assert(w_read(0236) == 34);

	// проверяем, что значение регистра уменьшилось на 2
	assert(reg[3] == 0236);
	assert(reg[5] == 0232);
}

void test_mode4_workwithsp()
{
	logger(TRACE, "Тест на работу 4 моды с 6 регистром(sp) вида movb -(R6), -(R3)\n");
   
	// setup
	reg[6] = 0350;  // ss
	reg[3] = 0356;	// dd
	b_write(0346, 34);
	b_write(0355, 12);

	Command test_cmd = parse_cmd(0114643);

	assert(dd.val == 12);
	assert(dd.adr == 0355);
	assert(ss.val == 34);
	assert(ss.adr == 0346);

	test_cmd.do_command();

	assert(b_read(0355) == 34); //проверяем прошло ли успешно копирование
	assert(b_read(0346) == 34);

	// проверяем, что значение регистра уменьшилось на 2 для регистра 3, на 1 для регистро 6(sp)
	assert(reg[3] == 0355);
	assert(reg[6] == 0346);
}
void test_mode5_toreg()
{
	logger(TRACE, "Тест моды 5 вида mov R5, @-(R3)\n");
	// setup
	reg[5] = 34;  // ss
	reg[3] = 0254;	// dd
	w_write(0252, 0256);
	w_write(0256, 12);

	Command test_cmd = parse_cmd(0010553);

	assert(dd.val == 12);
	assert(dd.adr == 0256);
	assert(ss.val == 34);
	assert(ss.adr == 5);

	test_cmd.do_command();

	assert(w_read(0256) == 34); //проверяем прошло ли успешно копирование

	assert(reg[3] == 0252);
	assert(reg[5] == 34);

}

void test_mode5_fromreg()
{
	logger(TRACE, "Тест моды 5 вида mov @-(R5), R3\n");
	// setup
	reg[5] = 0262;  // ss
	reg[3] = 12;	// dd
	w_write(0260, 0264);
	w_write(0264, 34);

	Command test_cmd = parse_cmd(0015503);

	assert(dd.val == 12);
	assert(dd.adr == 3);
	assert(ss.val == 34);
	assert(ss.adr == 0264);

	test_cmd.do_command();

	assert(reg[5] == 0260);
	assert(reg[3] == 34);

}

void test_mode5_fromregtoreg()
{
	logger(TRACE, "Тест моды 5 вида mov @-(R5), @-(R3)\n");
	// setup
	reg[5] = 0270;  // ss
	reg[3] = 0276;	// dd
	w_write(0266, 0272);
	w_write(0272, 34);
	w_write(0274, 0300);
	w_write(0300, 12);

	Command test_cmd = parse_cmd(0015553);

	assert(dd.val == 12);
	assert(dd.adr == 0300);
	assert(ss.val == 34);
	assert(ss.adr == 0272);

	test_cmd.do_command();

	assert(w_read(0272) == 34); //проверяем прошло ли успешно копирование
	assert(w_read(0300) == 34);

	assert(reg[3] == 0274);
	assert(reg[5] == 0266);
}

void test_mode6_toreg()
{
	logger(TRACE, "Тест моды 6 вида mov R6, 2(R3)\n");
	// setup
	reg[5] = 34;  // ss
	reg[3] = 0300;	// dd
	pc = 01000;
	w_write(01002, 02);
	w_write(0302, 12);

	Command test_cmd = parse_cmd(0010563);

	assert(dd.val == 12);
	assert(dd.adr == 0302);
	assert(ss.val == 34);
	assert(ss.adr == 5);

	test_cmd.do_command();

	assert(w_read(0302) == 34); //проверяем прошло ли успешно копирование

	assert(reg[3] == 0300);
	assert(reg[5] == 34);

}

void test_mode6_fromreg()
{
	logger(TRACE, "Тест моды 6 вида mov 4(R5), R3\n");
	// setup
	reg[5] = 0304;  // ss
	reg[3] = 12;	// dd
	pc = 01000;
	w_write(01002, 04);
	w_write(0310, 34);

	Command test_cmd = parse_cmd(0016503);

	assert(dd.val == 12);
	assert(dd.adr == 3);
	assert(ss.val == 34);
	assert(ss.adr == 0310);

	test_cmd.do_command();

	assert(reg[5] == 0304);
	assert(reg[3] == 34);

}

void test_mode6_fromregtoreg()
{
	logger(TRACE, "Тест моды 6 вида mov 6(R5), 10(R3)\n");
	// setup
	reg[5] = 0312;  // ss
	reg[3] = 0322;	// dd
	pc = 01000;
	w_write(01002, 06);
	w_write(0320, 34);
	w_write(01004, 010);
	w_write(0332, 12);

	Command test_cmd = parse_cmd(0016563);

	assert(dd.val == 12);
	assert(dd.adr == 0332);
	assert(ss.val == 34);
	assert(ss.adr == 0320);

	test_cmd.do_command();

	assert(w_read(0332) == 34); //проверяем прошло ли успешно копирование
	assert(w_read(0320) == 34);

	assert(reg[3] == 0322);
	assert(reg[5] == 0312);
}