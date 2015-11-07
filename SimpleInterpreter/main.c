#include <stdio.h>
#include <stdlib.h>

#include "Lexical.h"

////Keywords
//#define ALIAS 0x0
//#define CALL 0x1
//#define CMP 0x2
//#define DO 0x3
//#define WHILE 0x4
//#define REGA 0x5
//#define REGB 0x6
//#define REGC 0x7
//#define REGD 0x8
//#define STK 0x9
//#define TYPEDEF 0x10
//#define STRUCT 0x11
//#define PRINTV 0x12
//#define PRINTA 0x13


int main(void) {
	const char *path = "C:\\Programmering\\SimpleInterpreter\\syntax\\file.txt";
	char *cStr = readFile(path);
	tokenize(cStr);

	//freeMem();
	system("pause");
	return 0;
}