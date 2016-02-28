#include "HelpClass.h"
#include "HelpStructs.h"

#include <cmath>
#include <limits.h>

namespace Global {
	HelpClass::HelpClass() {}
	HelpClass::~HelpClass() {}

	bool HelpClass::StrCmp(const char *cStr1, const char *cStr2) {
		int i = 0;
		while (cStr1[i] != '\0' || cStr2[i] != '\0') {
			if (cStr1[i] != cStr2[i]) {
				return false;
			}
			++i;
		}
		return true;
	}

	const char* HelpClass::Strstr(const char *in, const char *find) {
		int inLength = strlen(in);
		int findLength = strlen(find);

		const char *it = in + inLength;

		while (it >= in) {

			if (*it == *(find + findLength - 1)) {

				bool ret = true;
				for (int i = 1; i < findLength; ++i) {

					if (*(it - i) != *(find + findLength - i - 1)) {
						ret = false;
						break;
					}
				}

				if (ret) {
					return it;
				}

			}
			--it;
		}

		return NULL;
	}

	bool HelpClass::FindSubStr(char *dest, const char* src, const char *find) {
		const char *result = Strstrr(src, find);

		if (result) {
			const int length = result - src + 2;

			memcpy(dest, result, length);
			dest[length] = '\0';

			return true;
		}

		return false;
	}

	const char* HelpClass::Strstrr(const char *cStr, const char *sub) {		
		int len = strlen(cStr) -1;
		int subLen = strlen(sub) -1;
		int k = subLen;
		
		for (int i = len; i >= 0; --i) {
			//first occurance.
			if (cStr[i] == sub[k]) {
				for (int k = subLen; k != -1; --k) {
					char c1 = cStr[i];
					char c2 = sub[k];
					if (cStr[i] == sub[k]) {
						if (k == 0) {
							return cStr + i;
						}
						--i;
					} else {
						k = subLen;
						break;
					}
				}
			}
		}
		
		return NULL;
		
		//
		//for (int i = len; i >= 0; --i) {
		//	if (cStr[i] == sub[subLen]) {
		//		while (k >= 0) {
		//			if (k == 0) {
		//				const char *c = cStr + i;
		//				printf("dsd");
		//				//return cStr + i;
		//			} else if (cStr[i] != sub[k]) {
		//				k = subLen;
		//				break;
		//			} else {
		//				--i;
		//				--k;
		//			}
		//		}
		//	}
		//}

		//int len = strlen(cStr);
		//int sublen = strlen(sub);

		//for (int i = 0; i < len; ++i) {

		//	int j = i;
		//	while (cStr[j] == sub[j - i]) {
		//		++j;
		//	}

		//	if ((j - i) == sublen) {
		//		return cStr + i;
		//	}
		//}
	}

	bool HelpClass::FindSubStrRev(char *dest, const char* src, const char *find) {
		const char *result = Strstrr(src, find);
		if (result) {
			const int len = strlen(result);
			memcpy(dest, result, len);
			dest[len] = '\0';
			return true;
		}

		return false;
		
		/*const char *result = Strstrr(src, find);

		if (result) {
			const int length = result - src + 1;

			memcpy(dest, src, length);
			dest[length] = '\0';

			return true;
		}

		return false;*/
	}

	int HelpClass::CheckForAlpha(const char *cStr) {
		int i = 0;
		while (cStr[i] != '\0') {
			if (!((cStr[i] >= 'a' && cStr[i] <= 'z') || (cStr[i] >= 'A' && cStr[i] <= 'Z'))) {
				return -1;
			}
			++i;
		}
		return 1;
	}

	int HelpClass::CheckForDigits(const char *cStr) {
		int i = 0;
		if (cStr[0] == '\0') { return -1; }
		while (cStr[i] != '\0') {
			if (!(cStr[i] >= '0' && cStr[i] <= '9')) {
				return -1;
			}
			++i;
		}
		return 1;
	}

	int HelpClass::CheckAliasNameConversion(const char *cStr) {
		if (!((cStr[0] >= 'a' && cStr[0] <= 'z') || (cStr[0] >= 'A' && cStr[0] <= 'Z'))) {
			return -1;
		}

		int len = strlen(cStr);
		if (len >= 2) {
			int i = 1;
			while (cStr[i] != '\0') {
				if (!((cStr[i] >= 'a' && cStr[i] <= 'z') || (cStr[i] >= 'A' && cStr[i] <= 'Z') || (cStr[i] >= '0' && cStr[i] <= '9'))) {
					return -1;
				}
				++i;
			}
			return 1;
		}

		return -1;
	}

	int HelpClass::IntLength(const int x) {
		if (x == 0) {
			return 1;
		}
		return floor(log10(abs(abs(x)))) + 1;
	}

	void HelpClass::ReplaceDotsWithSlashes(char *cStr) {
		int i = 0;
		while (cStr[i] != '\0') {
			if (cStr[i] == '.') {
				cStr[i] = '/';
			}
			++i;
		}
	}

	bool HelpClass::FindAnd(const char *cStr) {
		int i = 0;
		while (cStr[i] != '\0') {
			if (cStr[i] == '&') {
				return true;
			}
			++i;
		}
		return false;
	}

	bool HelpClass::IsNegativeNumber(const char *cStr) {
		Operator_s op0 = FindOperator(cStr, 0);
		if (op0.op[0] == '-' && op0.pos == 0) {
			return true;
		}
		return false;
	}

	Operator_s HelpClass::FindOperator(const char *cStr, const int startPos) {
		int len = strlen(cStr);
		int pl = -1;
		int mi = -1;
		int di = -1;
		int mu = -1;
		int eq = -1;
		int cmpEq = -1;
		int cmpNEq = -1;
		int cmpBi = -1;
		int cmpSm = -1;

		char buffer[INSTRUCTIONSIZE];

		if (FindSubStrRev(buffer, cStr, "==")) {
			if (buffer) {
				cmpEq = len - strlen(buffer) +1;
			}
		} else if (FindSubStrRev(buffer, cStr, "!=")) {
			if (buffer) {
				cmpNEq = len - strlen(buffer) +1;
			}
		} else if (FindSubStrRev(buffer, cStr, "=")) {
			if (buffer) {
				eq = len - strlen(buffer);
			}
		}

		if (FindSubStrRev(buffer, cStr, "*")) {
			if (buffer) {
				mu = len - strlen(buffer);
			}
		} 
		
		if (FindSubStrRev(buffer, cStr, "/")) {
			if (buffer) {
				di = len - strlen(buffer);
			}
		} 
		
		if (FindSubStrRev(buffer, cStr, "+")) {
			if (buffer) {
				pl = len - strlen(buffer);
			}
		} 
		
		if (FindSubStrRev(buffer, cStr, "-")) {
			if (buffer) {
				mi = len - strlen(buffer);
			}
		} 
		
		if (FindSubStrRev(buffer, cStr, ">")) {
			if (buffer) {
				cmpBi = len - strlen(buffer);
			}
		} 
		
		if (FindSubStrRev(buffer, cStr, "<")) {
			if (buffer) {
				cmpSm = len - strlen(buffer);
			}
		}

		Operator_s newOp;
		memset(newOp.op, '\0', 3);
		newOp.pos = -1;
		newOp.len = 0;

		if (pl > mi && pl > di && pl > mu && pl > eq && pl > cmpEq && pl > cmpNEq && pl > cmpBi && pl > cmpSm) {
			newOp.pos = pl;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		} else if (mi > pl && mi > di && mi > mu && mi > eq && mi > cmpEq && mi > cmpNEq && mi > cmpBi && mi > cmpSm) {
			newOp.pos = mi;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		} else if (di > pl && di > mi && di > mu && di > eq && di > cmpEq && di > cmpNEq && di > cmpBi && di > cmpSm) {
			newOp.pos = di;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		} else if (mu > pl && mu > di && mu > mi && mu > eq && mu > cmpEq && mu > cmpNEq && mu > cmpBi && mu > cmpSm) {
			newOp.pos = mu;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		} else if (eq > pl && eq > mu && eq > di && eq > mi && eq > cmpEq && eq > cmpNEq && eq > cmpBi && eq > cmpSm) {
			newOp.pos = eq;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		} else if (cmpEq > pl && cmpEq > mu && cmpEq > di && cmpEq > mi && cmpEq > cmpNEq && cmpEq > cmpBi && cmpEq > cmpSm) {
			newOp.pos = cmpEq;
			memcpy(newOp.op, buffer, 2);
			newOp.op[2] = '\0';
			newOp.len = 2;
		} else if (cmpNEq > pl && cmpNEq > mu && cmpNEq > di && cmpNEq > mi && cmpNEq > cmpEq && cmpNEq > cmpBi && cmpNEq > cmpSm) {
			newOp.pos = cmpNEq;
			memcpy(newOp.op, buffer, 2);
			newOp.op[2] = '\0';
			newOp.len = 2;
		} else if (cmpBi > pl && cmpBi > mu && cmpBi > di && cmpBi > mi && cmpBi > cmpEq && cmpBi > cmpNEq && cmpBi > cmpSm) {
			newOp.pos = cmpBi;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		} else if (cmpSm > pl && cmpSm > mu && cmpSm > di && cmpSm > mi && cmpSm > cmpEq && cmpSm > cmpNEq && cmpSm > cmpBi) {
			newOp.pos = cmpSm;
			memcpy(newOp.op, buffer, 1);
			newOp.op[1] = '\0';
			newOp.len = 1;
		}
		return newOp;
	}

	bool HelpClass::IsTextString(const char *cStr) {
		const char *txtStart = strstr(cStr, "\"");
		if (txtStart) {
			const char *txtEnd = strstr(txtStart + 1, "\"");
			if (txtEnd) {
				return true;
			}
		}
		return false;
	}

	int HelpClass::FindComment(const char *cStr) {
		const int len = strlen(cStr);
		for (int i = 0; i < len; ++i) {
			if (cStr[i] == '/' && cStr[i + 1] == '*') {
				for (int k = i; k < len; ++k) {
					if (cStr[k] == '*' && cStr[k + 1] == '/') {
						return k;
					}
				}
			}
		}
		return 0;
	}

	void HelpClass::EvalNegtiveExpression(char *buffer, const char *cStr) {
		int len = strlen(cStr);
		Operator_s op0 = FindOperator(cStr, 0);
		if (op0.pos == -1) {
			return;
		}
		memcpy(buffer, cStr, op0.pos);

		Operator_s op1 = FindOperator(buffer, 0);
		char op[1];
		if (op0.op[0] == '-' && op1.op[0] == '-') {
			op[0] = '+';
		} else if ((op0.op[0] == '-' && op1.op[0] == '+') || (op0.op[0] == '+' && op1.op[0] == '-')) {
			op[0] = '-';
		} else if (op0.op[0] == '-' && op1.op[0] == '=') {
			memcpy(buffer, cStr, len);
			buffer[len] = '\0';
			return;
		}
		
		memcpy(buffer, cStr, op0.pos -1);
		memcpy(buffer + op0.pos - 1, op, 1);
		memcpy(buffer + op0.pos, cStr + op0.pos +1, len - op0.pos);
		buffer[len - 1] = '\0';
	}
}