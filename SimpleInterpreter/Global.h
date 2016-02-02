#pragma once

#include "Defines.h"

#include <stdio.h>
#include <string.h>

struct Alias_s {
	char name[NAMESIZE];
	char type[TYPESIZE];
	char value[VALUESIZE];
	int len;
};

struct Index_s {
	char name[NAMESIZE];
	char type[TYPESIZE];
	int startPos;
	int len;
};

struct Operator_s {
	int pos;
	char op[3];
	int len;
};

class Global {
	public:
		Global();
		~Global();
		int strCmp(const char *cStr1, const char *cStr2);
		const char* strstrr(const char *in, const char *find);
		bool findSubStrRev(char *dest, const char* src, const char *find);
		int checkForAlpha(const char *cStr);
		int checkForDigits(const char *cStr);
		int intLength(int x);
		void replaceDotsWithSlashes(char *cStr);
		bool findAnd(char *cStr);
		bool isNegativeNumber(const char *cStr);
		Operator_s findOperator(const char *cStr, const int startPos);
};