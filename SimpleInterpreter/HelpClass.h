#pragma once

#include "Defines.h"

#include <stdio.h>
#include <string.h>

struct Operator_s;

class HelpClass {
	public:
		HelpClass();
		~HelpClass();
		static bool StrCmp(const char *cStr1, const char *cStr2);
		static const char* Strstrr(const char *in, const char *find);
		static bool FindSubStrRev(char *dest, const char* src, const char *find);
		static int CheckForAlpha(const char *cStr);
		static int CheckForDigits(const char *cStr);
		static int CheckAliasNameConversion(const char *cStr);
		static int IsOnlyDigits(const char *cStr);
		static int IntLength(int x);
		static void ReplaceDotsWithSlashes(char *cStr);
		static bool FindAnd(char *cStr);
		static bool IsNegativeNumber(const char *cStr);
		static Operator_s FindOperator(const char *cStr, const int startPos);
		static bool IsTextString(char *cStr);
		static int FindComment(char *cStr);
};