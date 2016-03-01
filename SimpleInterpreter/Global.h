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

struct Parts_s {
	char part[OUTPUTSIZE];
	int type; //1 = text. 2 = logic.
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

struct File_s {
	char name[PATHSIZE];
	int startPos = -1;
	int endPos = -1;
	int numberOfLines = -1;
};

class Global {
	public:
		Global();
		~Global();
		bool StrCmp(const char *cStr1, const char *cStr2);
		const char* Strstrr(const char *in, const char *find);
		bool FindSubStrRev(char *dest, const char* src, const char *find);
		int CheckForAlpha(const char *cStr);
		int CheckForDigits(const char *cStr);
		int CheckAliasNameConversion(const char *cStr);
		int IsOnlyDigits(const char *cStr);
		int IntLength(int x);
		void ReplaceDotsWithSlashes(char *cStr);
		bool FindAnd(char *cStr);
		bool IsNegativeNumber(const char *cStr);
		Operator_s FindOperator(const char *cStr, const int startPos);
		bool IsTextString(char *cStr);
		int FindComment(char *cStr);
};