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
		
		char* regularExpression(char *expression);
		char* calculateResult(char *expression);
		Alias_s parseKeywords(char *expression);
		void parsePrint(char *expression, Parts_s *parts, int &len);
		void setDatatype(Alias_s *aliasLhs, Alias_s aliasRhs);
		void setLength(Alias_s *aliasLhs, Alias_s aliasRhs);
		void updateIndex(Alias_s alias);

		void allocateMem();

		Heap heap;

		void createStack(size_t stackSize);
		void stackPop();
		void stackPopTop();
		int stackPushAt(char *cStr);
		int stackPushTop(char *cStr);
		Alias_s stackGetAt(char *cStr);
		Alias_s stackGetTop();

		Operator_s findOperator(const char *cStr, const int startPos);
		bool isNegativeNumber(const char *cStr);

		bool isAdress = false;

	private:
		char tmpStr[INSTRUCTIONSIZE];
		char tmpLhs[INSTRUCTIONSIZE];
		char tmpRhs[INSTRUCTIONSIZE];

		Global global;

		void updateAlias(Alias_s *alias);

		ErrorManager errorManager;
};

#endif //!PARSER_H