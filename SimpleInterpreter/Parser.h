#ifndef PARSER_H
#define PARSER_H

#include "Defines.h"
#include "Heap.h"
#include "Global.h"
#include "ErrorManager.h"

struct Alias_s;
struct Index_s;
struct Operator_s;

class Parser {
	public:
		Parser();
		~Parser();
		
		char* RegularExpression(char *expression);
		char* CalculateResult(char *expression);
		Alias_s ParseKeywords(char *expression);
		void ParsePrint(char *expression, Parts_s *parts, int &len);
		void SetDatatype(Alias_s *aliasLhs, Alias_s aliasRhs);
		void SetLength(Alias_s *aliasLhs, Alias_s aliasRhs);
		void UpdateIndex(Alias_s alias);

		void AllocateMem();

		void CreateStack(size_t stackSize);
		void StackPop();
		void StackPopTop();
		void StackPushAt(char *cStr);
		void StackPushTop(char *cStr);
		Alias_s StackGetAt(char *cStr);
		Alias_s StackGetTop();

		Operator_s findOperator(const char *cStr, const int startPos);
		bool isNegativeNumber(const char *cStr);

		bool isAdress = false;

		Heap heap;

	private:
		char tmpStr[INSTRUCTIONSIZE];
		char tmpLhs[INSTRUCTIONSIZE];
		char tmpRhs[INSTRUCTIONSIZE];

		Global global;

		void UpdateAlias(Alias_s *alias);

		ErrorManager errorManager;
};

#endif //!PARSER_H