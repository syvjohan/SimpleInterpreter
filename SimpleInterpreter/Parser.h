#ifndef PARSER_H
#define PARSER_H

#include "Defines.h"
#include "Heap.h"

namespace Global {
	struct Alias_s;
	struct Index_s;
	struct Operator_s;
	struct Parts_s;
}

namespace Partitioning {
	class Parser {
		public:
			Parser();
			~Parser();

			char* RegularExpression(char *expression);
			char* CalculateResult(const char *lhs, const char *rhs, const char *op);
			Global::Alias_s ParseKeywords(char *expression);
			void ParsePrint(char *expression, Global::Parts_s *parts, int &len);
			void SetDatatype(Global::Alias_s *aliasLhs, const Global::Alias_s aliasRhs);
			void SetLength(Global::Alias_s *aliasLhs, const Global::Alias_s aliasRhs);
			void UpdateIndex(const Global::Alias_s alias);

			void AllocateMem();

			void CreateStack(size_t stackSize);
			void StackPop();
			void StackPopTop();
			void StackPushAt(const char *cStr);
			void StackPushTop(const char *cStr);
			Global::Alias_s StackGetAt(const char *cStr);
			Global::Alias_s StackGetTop();

			Global::Operator_s findOperator(const char *cStr, const int startPos);
			bool isNegativeNumber(const char *cStr);

			bool isAdress = false;

			Memory::Heap heap;

		private:
			char tmpStr[INSTRUCTIONSIZE];
			char tmpLhs[INSTRUCTIONSIZE];
			char tmpRhs[INSTRUCTIONSIZE];
			char tmpOp[INSTRUCTIONSIZE];

			void UpdateAlias(Global::Alias_s *alias);
			int regularExpressionCallCounter = 0;
	};
}
#endif //!PARSER_H