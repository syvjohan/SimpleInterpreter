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

	void Parser::StackPushAt(const char *cStr) {
		int len = strlen(cStr) - 6;
		memcpy(tmpStr, cStr + 6, len);
		tmpStr[len] = '\0';

		const char *identifyType = strstr(cStr, "\"");
		const char *comma = strstr(cStr, ",");

		int success = 0;
		if (comma) {
			Global::Alias_s alias = { NULL, NULL, NULL, 0 };
			int len = strlen(tmpStr) - strlen(comma);
			memcpy(tmpLhs, tmpStr, len);
			tmpLhs[len] = '\0';

			const char *resLhs = RegularExpression(tmpLhs);
			int index;
			if (Global::HelpClass::CheckForDigits(resLhs) == 1) {
				index = atoi(resLhs);
			}

			len = strlen(comma) - 1;
			memcpy(tmpRhs, comma + 1, len);
			tmpRhs[len] = '\0';

			const char *resRhs = RegularExpression(tmpRhs);
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

	void Parser::StackPushTop(const char *cStr) {
		int len = strlen(cStr) - 7;
		memcpy(tmpStr, cStr + 7, len);
		tmpStr[len] = '\0';

		const char *identifyType = strstr(cStr, "\"");
		const char *res = RegularExpression(tmpStr);
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

		const char *identifyType = strstr(tmpStr, "\"");

		Global::Alias_s alias = { NULL, NULL, NULL, 0 };
		if (Global::HelpClass::CheckForDigits(tmpStr) == 1 && identifyType == NULL) {
			const int val = atoi(tmpStr);
			alias = heap.GetAt(val);
		}

		return alias;
	}

	Global::Alias_s Parser::StackGetTop() {
		return heap.GetTop();
	}

	//Recursive function if there is > 1 operator.
	char* Parser::RegularExpression(char *expression) {
		char buffer[INSTRUCTIONSIZE];
		char Buffer2[INSTRUCTIONSIZE];
		char *res = NULL;
		bool isRhsEval = true;

		Global::trimBothParanthesis(expression);

		int len = strlen(expression);

		Global::Operator_s op0 = Global::HelpClass::FindOperator(expression, 0);

		// No operator.
		if (op0.pos <= 0) {
			Global::Alias_s alias;
			alias = ParseKeywords(expression);

			if (strlen(alias.value) == 0) {
				memcpy(alias.value, expression, alias.len);
			}

			res = alias.value;
		} else {
			memcpy(buffer, expression, op0.pos - op0.len +1);
			buffer[op0.pos - op0.len +1] = '\0';

			//is there more than 1 operator?
			Global::Operator_s op1 = Global::HelpClass::FindOperator(buffer, 0);
			if (op1.pos == -1) {
				//Only one operator.
				const char *rhs = expression + op0.pos +1;
				res = CalculateResult(buffer, rhs, op0.op);
			} else {
				// More than one operator.
				const char *newExpression = expression + op1.pos +1;
				int lenNewExp = strlen(newExpression);
				int lenRhs = lenNewExp;
				int offset = 1;

				//Is there a '='
				char *eq = strstr(expression, "=");
				char *cmpEq = strstr(expression, "==");
				if (eq) {
					//Operators on left hand side of =.	
					if (!cmpEq) {
						if (!Global::HelpClass::StrCmp(op1.op, "=")) {	
							lenRhs = op0.pos - op1.pos - 1;
							op0 = op1;
							op1.pos = -1;
							offset = 0;
							isRhsEval = false;
						}
					} else {
						lenRhs = op0.pos - op1.pos - 2;
						op0 = op1;
						op1.pos = -1;
						offset = 2;
						isRhsEval = false;
					}
					//Operators on right hand side of = dont need extra care.
				}

				//Is't a negative number.
				if (!Global::HelpClass::IsNegativeNumber(newExpression)) {
					memcpy(Buffer2, expression + op0.pos + 1, lenRhs);
					Buffer2[lenRhs] = '\0';
					lenRhs = strlen(Buffer2);
					memcpy(buffer, expression + op1.pos +1, lenNewExp - lenRhs - offset);
					buffer[lenNewExp - lenRhs - offset] = '\0';
					res = CalculateResult(buffer, Buffer2, op0.op);
				} else {
					buffer[op0.pos -1] = '\0';
					Global::Operator_s op2 = Global::HelpClass::FindOperator(buffer, 0);
					if (op2.pos != -1) {
						//one or two operators directly after one another.
						if (op1.len == 1) {
							// =
							char *newNegExpression = expression + op2.pos + 1;
							const int lenNewNegExp = strlen(newExpression);

							memset(buffer, '\0', len);
							Global::HelpClass::EvalNegtiveExpression(buffer, newNegExpression);
							const char *rhs = buffer + op2.pos + 1;
							memcpy(op1.op, buffer + op2.pos, 1);
							buffer[op2.pos] = '\0';
							res = CalculateResult(buffer, rhs, op1.op);

							lenNewExp += lenNewNegExp;
						} else {
							// == || !=
							buffer[op0.pos - 2] = '\0';
							const char *rhs = expression + op1.pos + 1;
							return CalculateResult(buffer, rhs, op1.op);
						}
					} else {
						offset = 1;

						//Case x == -1.
						const char *exp = Global::HelpClass::Strstrr(expression, "==");
						if (exp) {
							offset = 2;
						} else {
							//Case x = -1.
							exp = Global::HelpClass::Strstrr(expression, "=");
						}

						const char *rhs = exp + offset; //jump over = operator.

						memcpy(tmpOp, exp, offset);
						tmpOp[offset] = '\0';

						const int lenRhs = strlen(rhs) +1;
						return CalculateResult(buffer, rhs, tmpOp);
					}
				}

				if (isRhsEval) {
					memcpy(buffer, expression, len - lenNewExp);
					buffer[len - lenNewExp] = '\0';
					strcat(buffer + (len - lenNewExp), res);
				} else {
					const int lenLhs = strlen(buffer) + strlen(Buffer2) + 1;
					const int lenRes = strlen(res);
					memset(buffer, '\0', INSTRUCTIONSIZE);
					memcpy(buffer, res, lenRes);
					memcpy(buffer + lenRes, expression + lenLhs, len - lenRes);
					buffer[len - lenRes + 1] = '\0';
				}

				RegularExpression(buffer);
			}
		}
		
		return res;
	}

	char* Parser::CalculateResult(const char *lhs, const char *rhs, const char *op) {
		tmpStr[0] = '\0'; //reseting...

		Global::Alias_s aliasLhs = ParseKeywords((char *)lhs);
		const Global::Alias_s aliasRhs = ParseKeywords((char *)rhs);
		
		if (Global::HelpClass::StrCmp(op, "=") && aliasLhs.name[0] != '\0') {
			//Updates alias
			if (lhs[0] == '&' || lhs[0] == '#') {
				SetDatatype(&aliasLhs, aliasRhs); //Change type in parameter 1.
				SetLength(&aliasLhs, aliasRhs); //Change type in parameter 1.
			} else {
				Error::ErrorManager::ErrorCode(Error::CODE_66);
			}
		} else {
			//Does not update alias.
			if (lhs[0] == '&' || lhs[0] == '#') {
				memcpy(aliasLhs.type, aliasRhs.type, strlen(aliasRhs.type));
				aliasLhs.len = aliasRhs.len;
			}
		}

		if ((lhs[0] == '#' && Global::HelpClass::StrCmp(op, "+")) || (lhs[0] == '#' && Global::HelpClass::StrCmp(op, "-"))) {
			//Remove # before calculation.
			int len = strlen(lhs) - 1;
			memcpy(aliasLhs.value, lhs + 1, len);
			aliasLhs.value[len] = '\0';
			if (Global::HelpClass::CheckForDigits(aliasLhs.value) && Global::HelpClass::StrCmp(aliasRhs.type, "int")) {
				//if user wannts to traverse memory addresses.
				if (Global::HelpClass::StrCmp(op, "+")) {
					sprintf(tmpStr, "%d", atoi(aliasLhs.value) + atoi(aliasRhs.value));
				} else {
					sprintf(tmpStr, "%d", atoi(aliasLhs.value) - atoi(aliasRhs.value));
				}
			}
			//Insert # after calculation.
			memcpy(tmpLhs, tmpStr, len);
			len = strlen(tmpLhs);
			memcpy(tmpStr, "#", 1);
			memcpy(tmpStr + 1, tmpLhs, len +1);
			tmpStr[len +1] = '\0';
			return tmpStr;
		}

		//do calculation
		//Both are digits
		if (Global::HelpClass::StrCmp(aliasLhs.type, "int") && Global::HelpClass::StrCmp(aliasRhs.type, "int")) {
			if (Global::HelpClass::StrCmp(op, "+")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) + atoi(aliasRhs.value));
			} else if (Global::HelpClass::StrCmp(op, "-")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) - atoi(aliasRhs.value));
			} else if (Global::HelpClass::StrCmp(op, "*")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) * atoi(aliasRhs.value));
			} else if (Global::HelpClass::StrCmp(op, "/")) {
				sprintf(tmpStr, "%d", atoi(aliasLhs.value) / atoi(aliasRhs.value));
				int v = atoi(tmpStr);
				if (v < 0) {
					tmpStr[0] = '0';
					tmpStr[1] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "==")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "!=")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "<")) {
				const int l = atoi(aliasLhs.value);
				const int r = atoi(aliasRhs.value);
				if (l < r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, ">")) {
				const int l = atoi(aliasLhs.value);
				const int r = atoi(aliasRhs.value);
				if (l > r) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "=")) {
				if (Global::HelpClass::CheckForAlpha(aliasRhs.value) == -1 && Global::HelpClass::CheckForDigits(aliasRhs.value) == -1 &&
					!Global::HelpClass::IsNegativeNumber(aliasRhs.value)) {

					Error::ErrorManager::ErrorCode(Error::CODE_3510);
				}

				int len = strlen(aliasRhs.value);
				memcpy(&aliasLhs.value, &aliasRhs.value, len);
				aliasLhs.value[len] = '\0';
				aliasLhs.len = sizeof(int);

				int a = 0;
				
				//Is it a memory address?
				if (aliasLhs.name[0] == '\0') {
					len = strlen(lhs) -1;
					memcpy(tmpStr, lhs + 1, len);
					a = atoi(tmpStr);
					heap.InsertAliasAt(a, aliasLhs);

				} else {
					a = heap.GetAddress(aliasLhs.name);
					heap.InsertAliasAt(a, aliasLhs);
				}

				//Update index.
				Global::Index_s index;
				index.len = aliasLhs.len;
				const int lenName = strlen(aliasLhs.name);
				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.startPos = a;
				const int lenType = strlen(aliasLhs.type);
				memcpy(index.type, aliasLhs.type, lenType);
				index.type[lenType] = '\0';

				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.len = aliasLhs.len;

				//Is it heapIndex or structIndex.
				const char *type = strstr(aliasLhs.name, ".");
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
			if (Global::HelpClass::StrCmp(op, "+")) {
				return strcat(aliasLhs.value, aliasRhs.value);
			} else if (Global::HelpClass::StrCmp(op, "-")) {
				//Wrong syntax do CRASH!!!
			} else if (Global::HelpClass::StrCmp(op, "*")) {
				//Wrong syntax do CRASH!!!
			} else if (Global::HelpClass::StrCmp(op, "/")) {
				//Wrong syntax do CRASH!!!
			} else if (Global::HelpClass::StrCmp(op, "==")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "!=")) {
				if (Global::HelpClass::StrCmp(aliasLhs.value, aliasRhs.value)) {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				} else {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "<")) {
				if (aliasLhs.len < aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, ">")) {
				if (aliasLhs.len > aliasRhs.len) {
					memcpy(tmpStr, "true", 4);
					tmpStr[4] = '\0';
				} else {
					memcpy(tmpStr, "false", 5);
					tmpStr[5] = '\0';
				}
			} else if (Global::HelpClass::StrCmp(op, "=")) {
				memcpy(&aliasLhs.value, &aliasRhs.value, strlen(aliasRhs.value));
				aliasLhs.len = aliasRhs.len;
				const int a = heap.GetAddress(aliasLhs.name);
				heap.InsertAliasAt(a, aliasLhs);

				//Update index.
				Global::Index_s index;
				index.len = aliasLhs.len;
				const int lenName = strlen(aliasLhs.name);
				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.startPos = a;
				const int lenType = strlen(aliasLhs.type);
				memcpy(index.type, aliasLhs.type, lenType);
				index.type[lenType] = '\0';

				memcpy(index.name, aliasLhs.name, lenName);
				index.name[lenName] = '\0';

				index.len = aliasLhs.len;

				//Is it heapIndex or structIndex.
				const char *type = strstr(aliasLhs.name, ".");
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

		const char *structName = heap.GetFullNameStructMember(exp);
		if (structName) {
			int len = strlen(structName);
			memcpy(exp, structName, len);
			exp[len] = '\0';
		}

		Global::Alias_s a = heap.GetAlias(exp); //Calculator.calc.valSecond
		const char *stack = strstr(exp, ":stk");
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
			const char *popTop = strstr(stack, "popTop");
			const char *pop = strstr(stack, "pop");
			const char *pushAt = strstr(stack, "pushAt");
			const char *pushTop = strstr(stack, "pushTop");
			const char *getAt = strstr(stack, "getAt");
			const char *getTop = strstr(stack, "getTop");

			const char *identifyType = strstr(exp, "\"");

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
			const int a = atoi(tmpStr);
			alias = heap.GetAlias(a);
			if (alias.len == 0 && alias.name[0] == '\0' && alias.type[0] == '\0' && alias.value[0] == '\0') {
				const char c = heap.GetValueAt(a);
				if (c != '\0') {
					alias.value[0] = c;
				}
			}

		} else if (address && isAdress) {
			Global::trimBothParanthesis(address);
			len = strlen(address) - 1;
			memcpy(alias.type, "address", 7);
			memcpy(alias.name, tmpStr, len);

			const int digits = heap.GetAddress(tmpStr);
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

			const int digits = heap.GetAddress(tmpStr);
			sprintf(alias.value, "%d", digits);

		} else if (and && dot && !isAdress) {
			Global::trimBothParanthesis(and);

			//struct
			const char *d = strstr(dot + 1, ".");
			if (d) {
				memcpy(tmpLhs, dot + 1, strlen(dot) - strlen(d) - 1);
			} else {
				memcpy(tmpLhs, dot, strlen(dot));
			}

			Global::Index_s indexStruct = heap.GetStructIndex(tmpLhs);

			//alias
			memcpy(tmpRhs, and + 1, strlen(and));

			Global::Index_s indexAlias = heap.GetStructIndex(tmpRhs);

			const int a = indexStruct.startPos + indexAlias.startPos;
			Global::Index_s newIndex;
			newIndex.len = indexAlias.len;
			newIndex.startPos = a;

			const int lenStructName = strlen(tmpRhs);
			memcpy(newIndex.name, tmpRhs, lenStructName);
			newIndex.name[lenStructName] = '\0';

			const int lenType = strlen(indexAlias.type);
			memcpy(newIndex.type, indexAlias.type, lenType);
			newIndex.type[lenType] = '\0';

			const bool isUpdated = heap.UpdateStructIndex(newIndex, newIndex.name);

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
		const int address = heap.GetAddress(alias->name); //String.string.pPair.first
		assert(address != -1);

		heap.InsertAliasAt(address, *alias);			 //String.pPair.second
	}

	void Parser::ParsePrint(char *expression, Global::Parts_s *parts, int &len) {
		int lenParts = 0;
		char part[OUTPUTSIZE];
		int oldEndPoint = 0;
		int endPoint = 0;
		int lenPart = 0;
		bool isHit = false;
		const int lenExp = strlen(expression);

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