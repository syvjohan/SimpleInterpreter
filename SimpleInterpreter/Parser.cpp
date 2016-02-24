#include "Parser.h"
#include "HelpHeaders.h"
#include "ErrorHeaders.h"

namespace Partitioning {
	Parser::Parser() {}

	Parser::~Parser() {}

	void Parser::StackPopTop() {
		heap.PopTop();
	}

	void Parser::StackPop() {
		heap.Pop();
	}

	void Parser::StackPushAt(char *cStr) {
		int len = strlen(cStr) - 6;
		memcpy(tmpStr, cStr + 6, len);
		tmpStr[len] = '\0';

		char *identifyType = strstr(cStr, "\"");
		char *comma = strstr(cStr, ",");

		int success = 0;
		if (comma) {
			Global::Alias_s alias = { NULL, NULL, NULL, 0 };
			int len = strlen(tmpStr) - strlen(comma);
			memcpy(tmpLhs, tmpStr, len);
			tmpLhs[len] = '\0';

			char *resLhs = RegularExpression(tmpLhs);
			int index;
			if (Global::HelpClass::CheckForDigits(resLhs) == 1) {
				index = atoi(resLhs);
			}

			len = strlen(comma) - 1;
			memcpy(tmpRhs, comma + 1, len);
			tmpRhs[len] = '\0';

			char *resRhs = RegularExpression(tmpRhs);
			len = strlen(resRhs);

			if (identifyType) {
				memcpy(alias.type, "string", 6);
				alias.type[len] = '\0';
				alias.len = len;

			} else if (Global::HelpClass::CheckForDigits(resRhs) == 1 && identifyType == NULL) {
				memcpy(alias.type, "int", 3);
				alias.type[3] = '\0';
				alias.len = len;
			} else {
				//Syntax is wrong, DO CRASH!!
			}
			memcpy(alias.value, resRhs, alias.len);
			alias.value[alias.len] = '\0';

			heap.PushAt(index, alias);

		} else {
			//Only one parameter, DO CRASH!!
		}
	}

	void Parser::StackPushTop(char *cStr) {
		int len = strlen(cStr) - 7;
		memcpy(tmpStr, cStr + 7, len);
		tmpStr[len] = '\0';

		char *identifyType = strstr(cStr, "\"");
		char *res = RegularExpression(tmpStr);
		len = strlen(res);

		Global::Alias_s alias;
		if (identifyType) {
			memcpy(alias.type, "string", 6);
			alias.type[6] = '\0';

			alias.len = len;
		} else if (Global::HelpClass::CheckForDigits(res) == 1 && identifyType == NULL) {
			memcpy(alias.type, "int", 3);
			alias.type[3] = '\0';

			alias.len = len;
		}
		memcpy(alias.name, "\0", 1);
		memcpy(alias.value, res, alias.len);
		alias.value[alias.len] = '\0';

		heap.PushTop(alias);
	}

	Global::Alias_s Parser::StackGetAt(const char *cStr) {
		int len = strlen(cStr) - 5;
		memcpy(tmpStr, cStr + 5, len);
		tmpStr[len] = '\0';

		char *identifyType = strstr(tmpStr, "\"");

		Global::Alias_s alias = { NULL, NULL, NULL, 0 };
		if (Global::HelpClass::CheckForDigits(tmpStr) == 1 && identifyType == NULL) {
			int val = atoi(tmpStr);
			alias = heap.GetAt(val);
		}

		return alias;
	}

	Global::Alias_s Parser::StackGetTop() {
		return heap.GetTop();
	}

	char* Parser::RegularExpression(char *expression) {
		Global::trimBothParanthesis(expression);
		int len = strlen(expression);
		char str[INSTRUCTIONSIZE];
		char lhs[INSTRUCTIONSIZE];
		char rhs[INSTRUCTIONSIZE];

		//If there is no operators return expression(end condition).
		Global::Operator_s op0 = Global::HelpClass::FindOperator(expression, 0);
		if (op0.pos == -1) {
			Global::Alias_s a;
			//Is it a stack call?
			a = ParseKeywords(expression);

			if (strlen(a.value) == 0) {
				memcpy(a.value, expression, a.len);
			}
			return a.value;
		}

		//Negative number.
		if (Global::HelpClass::IsNegativeNumber(expression)) {
			return expression;
		}

		Global::Operator_s op1 = Global::HelpClass::FindOperator(expression, op0.pos + 1);
		if (op1.pos != -1) {

			//Negative number.
			if (op0.pos != -1 && op1.op[0] == '-' && op1.pos == op0.pos + 1) {
				len = strlen(expression);
			} else {
				//If there is a second operator set len = operator.
				len = op0.pos + 1;
			}
		}

		memcpy(lhs, expression, len);
		lhs[len] = '\0';

		int rhsLen = strlen(expression) - len;
		memcpy(rhs, expression + len, rhsLen);
		rhs[rhsLen] = '\0';

		if (regularExpressionCallCounter == 0) {
			if (op0.pos == 0) {
				Error::ErrorManager::ErrorCode(Error::CODE_71);
			} else if (op0.pos == len - 1 && rhsLen == 0) {
				Error::ErrorManager::ErrorCode(Error::CODE_72);
			}
		}

		if (len > 0) {
			char *res;
			if (op0.pos != -1 && op1.pos != -1) {
				res = CalculateResult(rhs);

				strcat(lhs, res);
				len = strlen(lhs);
				memcpy(str, lhs, len);
				str[len] = '\0';

			} else {
				res = CalculateResult(lhs);

				len = strlen(res);
				memcpy(str, res, len);
				str[len] = '\0';

				strcat(str, rhs);
				str[len + strlen(rhs)] = '\0';
			}

			//Negative number.
			if (Global::HelpClass::IsNegativeNumber(str)) {
				return str;
			}

			//recursion.
			len = strlen(str);
			memcpy(tmpStr, str, len);
			tmpStr[len] = '\0';

			++regularExpressionCallCounter;
			RegularExpression(tmpStr);

			return tmpStr;
		}
		regularExpressionCallCounter = 0;
		return "";
	}

	char* Parser::CalculateResult(const char *exp) {
		tmpStr[0] = '\0'; //reseting...

		Global::Operator_s op0 = Global::HelpClass::FindOperator(exp, 0);

		char lhs[INSTRUCTIONSIZE];
		char rhs[INSTRUCTIONSIZE];

		int rhsLen = strlen(exp) - op0.pos;
		if (op0.len == 1) {
			memcpy(lhs, exp, op0.pos);
			lhs[op0.pos] = '\0';

			memcpy(rhs, exp + op0.pos + op0.len, rhsLen);
			rhs[rhsLen] = '\0';

		} else if (op0.len == 2) {
			memcpy(lhs, exp, op0.pos - 1);
			lhs[op0.pos - 1] = '\0';

			memcpy(rhs, exp + op0.pos + op0.len - 1, rhsLen);
			rhs[rhsLen] = '\0';
		}

		Global::Alias_s aliasLhs = ParseKeywords(lhs);
		Global::Alias_s aliasRhs = ParseKeywords(rhs);

		if (op0.op[0] == '=' && op0.op[1] != '=') {
			if (lhs[0] == '&') {
				SetDatatype(&aliasLhs, aliasRhs); //Change type in parameter 1.
				SetLength(&aliasLhs, aliasRhs); //Change type in parameter 1.
			} else {
				Error::ErrorManager::ErrorCode(Error::CODE_66);
			}
		}

		//do calculation
		//Both are digits
		if (Global::HelpClass::StrCmp(aliasLhs.type, "int") && Global::HelpClass::StrCmp(aliasRhs.type, "int")) {
			if (Global::HelpClass::StrCmp(op0.op, "+")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) + atoi(aliasRhs.value));
			} else if (Global::HelpClass::StrCmp(op0.op, "-")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) - atoi(aliasRhs.value));
			} else if (Global::HelpClass::StrCmp(op0.op, "*")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) * atoi(aliasRhs.value));
			} else if (Global::HelpClass::StrCmp(op0.op, "/")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) / atoi(aliasRhs.value));
				int v = atoi(tmpStr);
				if (v < 0) {
					tmpStr[0] = '0';
					tmpStr[1] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "==")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "!=")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "<")) {
				int l = atoi(aliasLhs.value);
				int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, ">")) {
				int l = atoi(aliasLhs.value);
				int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "=")) {
				if (Global::HelpClass::CheckForAlpha(aliasRhs.value) == -1 && Global::HelpClass::CheckForDigits(aliasRhs.value) == -1) {
					Error::ErrorManager::ErrorCode(Error::CODE_3510);
				}

				int len = strlen(aliasRhs.value);
				memcpy(&aliasLhs.value, &aliasRhs.value, len);
				aliasLhs.value[len] = '\0';
				aliasLhs.len = sizeof(int);
				int a = heap.GetAddress(aliasLhs.name);
				heap.InsertAliasAt(a, aliasLhs);

				//Update index.
				Global::Index_s index;
				index.len = aliasLhs.len;
				int lenName = strlen(aliasLhs.name);
				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.startPos = a;
				int lenType = strlen(aliasLhs.type);
				memcpy(index.type, aliasLhs.type, lenType);
				index.type[lenType] = '\0';

				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.len = aliasLhs.len;

				//Is it heapIndex or structIndex.
				char *type = strstr(aliasLhs.name, ".");
				if (type) {
					heap.UpdateStructIndex(index, index.name);
					heap.UpdateStructHeaderPointer(index);
				} else {
					heap.UpdateHeapIndex(index);
				}

				len = strlen(aliasLhs.value);
				memcpy(tmpStr, aliasLhs.value, len);
				tmpStr[len] = '\0';

			} else {
				//Wrong syntax do CRASH!!!
			}
		}
		//Both are text strings.
		else if (Global::HelpClass::StrCmp(aliasLhs.type, "string") && Global::HelpClass::StrCmp(aliasRhs.type, "string")) {
			if (Global::HelpClass::StrCmp(op0.op, "+")) {
				return strcat(aliasLhs.value, aliasRhs.value);
			} else if (Global::HelpClass::StrCmp(op0.op, "-")) {
				//Wrong syntax do CRASH!!!
			} else if (Global::HelpClass::StrCmp(op0.op, "*")) {
				//Wrong syntax do CRASH!!!
			} else if (Global::HelpClass::StrCmp(op0.op, "/")) {
				//Wrong syntax do CRASH!!!
			} else if (Global::HelpClass::StrCmp(op0.op, "==")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "!=")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "<")) {
				if (aliasLhs.len < aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, ">")) {
				if (aliasLhs.len > aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op0.op, "=")) {
				memcpy(&aliasLhs.value, &aliasRhs.value, strlen(aliasRhs.value));
				aliasLhs.len = aliasRhs.len;
				int a = heap.GetAddress(aliasLhs.name);
				heap.InsertAliasAt(a, aliasLhs);

				//Update index.
				Global::Index_s index;
				index.len = aliasLhs.len;
				int lenName = strlen(aliasLhs.name);
				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.startPos = a;
				int lenType = strlen(aliasLhs.type);
				memcpy(index.type, aliasLhs.type, lenType);
				index.type[lenType] = '\0';

				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.len = aliasLhs.len;

				//Is it heapIndex or structIndex.
				char *type = strstr(aliasLhs.name, ".");
				if (type) {
					heap.UpdateStructIndex(index, index.name);
					heap.UpdateStructHeaderPointer(index);
				} else {
					heap.UpdateHeapIndex(index);
				}

				int len = strlen(aliasLhs.value);
				memcpy(tmpStr, aliasLhs.value, len);
				tmpStr[len] = '\0';
			}
		} else {
			//errorManager.ErrorCode(CODE_100);
		}
		return tmpStr;
	}

	Global::Alias_s Parser::ParseKeywords(char *exp) {
		Global::Alias_s alias = { NULL, NULL, NULL, 0 };

		if (exp[0] == '\0') {
			return alias;
		}

		char *structName = heap.GetFullNameStructMember(exp);
		if (structName) {
			int len = strlen(structName);
			memcpy(exp, structName, len);
			exp[len] = '\0';
		}

		Global::Alias_s a = heap.GetAlias(exp);
		char *stack = strstr(exp, ":stk");
		char *address = strstr(exp, "#");
		char *and = strstr(exp, "&");
		char *dot = strstr(exp, ".");

		int len = strlen(exp);

		if (a.len > 0) {
			memcpy(&alias, &a, sizeof(a));

		} else if (a.name[0] != '\0') {
			Global::Index_s index = heap.GetStructIndex(a.name);
			alias = heap.GetAlias(index.startPos);
		} else if (stack) {
			char *popTop = strstr(stack, "popTop");
			char *pop = strstr(stack, "pop");
			char *pushAt = strstr(stack, "pushAt");
			char *pushTop = strstr(stack, "pushTop");
			char *getAt = strstr(stack, "getAt");
			char *getTop = strstr(stack, "getTop");

			char *identifyType = strstr(exp, "\"");

			if (popTop) {
				StackPopTop();
			} else if (pop) {
				StackPop();
			} else if (pushAt) {
				StackPushAt(stack);
			} else if (pushTop) {
				StackPushTop(pushTop);
			} else if (getAt) {
				alias = StackGetAt(getAt);
			} else if (getTop) {
				alias = heap.GetTop();
			} else {
				//unsupported stack command.
				Error::ErrorManager::ErrorCode(Error::CODE_60);
			}

		} else if (address && !isAdress) {
			Global::trimBothParanthesis(address);
			len = strlen(address) - 1;
			memcpy(tmpStr, address + 1, len);
			tmpStr[len] = '\0';
			int a = atoi(tmpStr);
			alias = heap.GetAlias(a);

		} else if (address && isAdress) {
			Global::trimBothParanthesis(address);
			len = strlen(address) - 1;
			memcpy(alias.type, "address", 7);
			memcpy(alias.name, tmpStr, len);

			int digits = heap.GetAddress(tmpStr);
			sprintf(alias.value, "%d", digits);

			alias.len = strlen(alias.value);

		} else if (!and && dot && !isAdress) {
			int lenRhs = strlen(dot) - 1;
			memcpy(tmpRhs, dot + 1, lenRhs);
			tmpRhs[lenRhs] = '\0';

			int lenLhs = len - lenRhs - 1;
			memcpy(tmpLhs, exp, lenLhs);
			tmpLhs[lenLhs] = '\0';
			Global::Index_s indexType = heap.GetStructIndex(tmpLhs);

			int len1 = strlen(tmpLhs);
			int len2 = strlen(indexType.type);
			int lendot = strlen(dot);
			memcpy(tmpStr, indexType.type, len2);
			memcpy(tmpStr + len2, ".", 1);
			len2 += 1;
			memcpy(tmpStr + len2, tmpLhs, len1);
			memcpy(tmpStr + len2 + len1, dot, lendot);
			tmpStr[len1 + len2 + lendot] = '\0';

			alias = heap.GetAlias(tmpStr);

		} else if (and && !dot && !isAdress) {
			Global::trimBothParanthesis(and);
			len = strlen(and) - 1;
			memcpy(tmpStr, and + 1, len);
			tmpStr[len] = '\0';
			alias = heap.GetAlias(tmpStr);

		} else if (and && !dot && isAdress) {
			Global::trimBothParanthesis(and);
			len = strlen(and) - 1;
			memcpy(tmpStr, and + 1, len);
			tmpStr[len] = '\0';
			alias.len = len;
			memcpy(alias.type, "address", 6);
			memcpy(alias.name, tmpStr, alias.len);

			int digits = heap.GetAddress(tmpStr);
			sprintf(alias.value, "%d", digits);

		} else if (and && dot && !isAdress) {
			Global::trimBothParanthesis(and);

			//struct
			char *d = strstr(dot + 1, ".");
			if (d) {
				memcpy(tmpLhs, dot + 1, strlen(dot) - strlen(d) - 1);
			} else {
				memcpy(tmpLhs, dot, strlen(dot));
			}

			Global::Index_s indexStruct = heap.GetStructIndex(tmpLhs);

			//alias
			memcpy(tmpRhs, and + 1, strlen(and));

			Global::Index_s indexAlias = heap.GetStructIndex(tmpRhs);

			int a = indexStruct.startPos + indexAlias.startPos;
			Global::Index_s newIndex;
			newIndex.len = indexAlias.len;
			newIndex.startPos = a;

			int lenStructName = strlen(tmpRhs);
			memcpy(newIndex.name, tmpRhs, lenStructName);
			newIndex.name[lenStructName] = '\0';

			int lenType = strlen(indexAlias.type);
			memcpy(newIndex.type, indexAlias.type, lenType);
			newIndex.type[lenType] = '\0';

			bool isUpdated = heap.UpdateStructIndex(newIndex, newIndex.name);

			if (!isUpdated) {
				heap.InsertStructIndex(newIndex);
			}

			alias = heap.GetAlias(newIndex.name);

		} else if (strstr(exp, "\"") == NULL) {
			Global::trimBothParanthesis(exp);
			//Digits
			alias.len = strlen(exp);
			memcpy(alias.value, exp, alias.len);
			alias.value[alias.len] = '\0';

			memcpy(alias.type, "int", 3);
			alias.type[3] = '\0';

		} else if (strstr(exp, "\"")) {
			Global::trimBothParanthesis(exp);
			//Letters
			Global::trimText(exp);
			alias.len = strlen(exp);
			memcpy(alias.value, exp, alias.len);
			alias.value[alias.len] = '\0';

			memcpy(alias.type, "string", 6);
			alias.type[6] = '\0';
		}

		return alias;
	}

	void Parser::SetDatatype(Global::Alias_s *aliasLhs, Global::Alias_s aliasRhs) {
		if (!Global::HelpClass::StrCmp(aliasRhs.type, "")) {
			if (Global::HelpClass::StrCmp(aliasRhs.type, "int")) {
				memcpy(aliasLhs->type, "int", 3);
				aliasLhs->type[3] = '\0';

			} else if (Global::HelpClass::StrCmp(aliasRhs.type, "string")) {
				memcpy(aliasLhs->type, "string", 6);
				aliasLhs->type[6] = '\0';
			}
		}
	}

	void Parser::SetLength(Global::Alias_s *aliasLhs, Global::Alias_s aliasRhs) {
		aliasLhs->len = aliasRhs.len;
		UpdateAlias(aliasLhs);
	}

	void Parser::UpdateAlias(Global::Alias_s *alias) {
		int address = heap.GetAddress(alias->name);
		heap.InsertAliasAt(address, *alias);
	}

	void Parser::ParsePrint(char *expression, Global::Parts_s *parts, int &len) {
		int lenParts = 0;
		char part[OUTPUTSIZE];
		int oldEndPoint = 0;
		int endPoint = 0;
		int lenPart = 0;
		bool isHit = false;
		int lenExp = strlen(expression);

		Global::trimWhitespacesExceptInTextString(expression);

		do {
			for (int i = endPoint; i <= lenExp; ++i) {
				if (expression[i] == ',') {
					oldEndPoint = endPoint;
					endPoint = i;
					isHit = true;
					break;
				}
			}

			if (!isHit) {
				oldEndPoint = endPoint;
				endPoint = lenExp;
			}
			isHit = false;

			lenPart = endPoint - oldEndPoint;
			memcpy(part, expression + oldEndPoint, lenPart);
			part[lenPart] = '\0';

			if (Global::HelpClass::IsTextString(part)) {
				parts[lenParts].type = 1;
				Global::trimText(part);
			} else {
				Global::trimWhitespaces(part);
				parts[lenParts].type = 2;
			}
			memcpy(&parts[lenParts].part, part, lenPart);
			parts[lenParts].len = lenPart;
			++lenParts;
			endPoint += 1; //Jump over comma.
		} while (endPoint < lenExp);

		len = lenParts;
	}
}