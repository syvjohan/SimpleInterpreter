#pragma once

#include "Defines.h"

#include <stdio.h>
#include <string.h>

namespace Global {
	struct Operator_s;

	class HelpClass {
		public:
		HelpClass();
		~HelpClass();
		static bool StrCmp(const char *cStr1, const char *cStr2);
		static const char* Strstrr(const char *in, const char *find);
		static const char* Strstr(const char *cStr, const char *sub);
		static bool FindSubStrRev(char *dest, const char* src, const char *find);
		static bool FindSubStr(char *dest, const char* src, const char *find);
		static int CheckForAlpha(const char *cStr);
		static int CheckForDigits(const char *cStr);
		static int CheckAliasNameConversion(const char *cStr);
		static int IsOnlyDigits(const char *cStr);
		static int IntLength(int x);
		static void ReplaceDotsWithSlashes(char *cStr);
		static bool FindAnd(const char *cStr);
		static bool IsNegativeNumber(const char *cStr);
		static Operator_s FindOperator(const char *cStr, const int startPos);
		static bool IsTextString(const char *cStr);
		static int FindComment(const char *cStr);
		static void EvalNegtiveExpression(char *result, const char *cStr);
	};
}