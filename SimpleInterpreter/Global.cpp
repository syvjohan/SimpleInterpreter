#include "Global.h"
#include <cmath>
#include <limits.h>

Global::Global() {}
Global::~Global() {}

int Global::strCmp(const char *cStr1, const char *cStr2) {
	if (cStr1 == NULL || cStr2 == NULL) {
		return 0;
	}

	if (cStr1 == 0 || cStr2 == 0) {
		return 0;
	}

	int i = 0;
	while (cStr1[i] != '\0' || cStr2[i] != '\0') {
		if (cStr1[i] != cStr2[i]) {
			return 0;
		}
		++i;
	}
	return 1;
}

const char* Global::strstrr(const char *in, const char *find) {
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

bool Global::findSubStrRev(char *dest, const char* src, const char *find) {
	const char *result = strstrr(src, find);

	if (result) {
		const int length = result - src + 1;

		memcpy(dest, src, length);
		dest[length] = '\0';

		return true;
	}

	return false;
}

int Global::checkForAlpha(const char *cStr) {
	int i = 0;
	while (cStr[i] != '\0') {
		if (!((cStr[i] >= 'a' && cStr[i] <= 'z') || (cStr[i] >= 'A' && cStr[i] <= 'Z'))) {
			return -1;
		}
		++i;
	}
	return 1;
}

int Global::checkForDigits(const char *cStr) {
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

int Global::checkAliasNameConversion(const char *cStr) {
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

int Global::intLength(int x) {
	if (x == 0) {
		return 1;
	}
	return floor(log10(abs(abs(x)))) + 1;
}

void Global::replaceDotsWithSlashes(char *cStr) {
	int i = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] == '.') {
			cStr[i] = '/';
		}
		++i;
	}
}

bool Global::findAnd(char *cStr) {
	int i = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] == '&') {
			return true;
		}
		++i;
	}
	return false;
}

bool Global::isNegativeNumber(const char *cStr) {
	Operator_s op0 = findOperator(cStr, 0);
	if (op0.op[0] == '-' && op0.pos == 0) {
		Operator_s op1 = findOperator(cStr, op0.len);
		if (op1.len == 0) {
			return true;
		}
	}
	return false;
}

Operator_s Global::findOperator(const char *cStr, const int startPos) {
	int len = strlen(cStr);
	int pl = INT_MAX;
	int mi = INT_MAX;
	int di = INT_MAX;
	int mu = INT_MAX;
	int eq = INT_MAX;
	int cmpEq = INT_MAX;
	int cmpNEq = INT_MAX;
	int cmpBi = INT_MAX;
	int cmpSm = INT_MAX;

	char buffer[INSTRUCTIONSIZE];

	if (findSubStrRev(buffer, cStr + startPos, "=")) {
		if (buffer) {
			eq = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "+")) {
		if (buffer) {
			pl = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "-")) {
		if (buffer) {
			mi = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "*")) {
		if (buffer) {
			mu = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "/")) {
		if (buffer) {
			di = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, ">")) {
		if (buffer) {
			cmpBi = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "<")) {
		if (buffer) {
			cmpSm = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "==")) {
		if (buffer) {
			cmpEq = strlen(buffer) - 1;
		}
	}

	if (findSubStrRev(buffer, cStr + startPos, "!=")) {
		if (buffer) {
			cmpNEq = strlen(buffer) - 1;
		}
	}

	Operator_s newOp;
	memset(newOp.op, '\0', 3);
	newOp.pos = -1;
	newOp.len = 0;

	if (pl < mi && pl < di && pl < mu && pl < eq && pl < cmpEq && pl < cmpNEq && pl < cmpBi && pl < cmpSm) {
		newOp.pos = pl;
		memcpy(newOp.op, buffer + pl, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (mi < pl && mi < di && mi < mu && mi < eq && mi < cmpEq && mi < cmpNEq && mi < cmpBi && mi < cmpSm) {
		newOp.pos = mi;
		memcpy(newOp.op, buffer + mi, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (di < pl && di < mi && di < mu && di < eq && di < cmpEq && di < cmpNEq && di < cmpBi && di < cmpSm) {
		newOp.pos = di;
		memcpy(newOp.op, buffer + di, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (mu < pl && mu < di && mu < mi && mu < eq && mu < cmpEq && mu < cmpNEq && mu < cmpBi && mu < cmpSm) {
		newOp.pos = mu;
		memcpy(newOp.op, buffer + mu, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (eq < pl && eq < mu && eq < di && eq < mi && eq < cmpEq && eq < cmpNEq && eq < cmpBi && eq < cmpSm) {
		newOp.pos = eq;
		memcpy(newOp.op, buffer + eq, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (cmpEq < pl && cmpEq < mu && cmpEq < di && cmpEq < mi && cmpEq < cmpNEq && cmpEq < cmpBi && cmpEq < cmpSm) {
		newOp.pos = cmpEq;
		memcpy(newOp.op, buffer + cmpEq - 1, 2);
		newOp.op[2] = '\0';
		newOp.len = 2;
	} else if (cmpNEq < pl && cmpNEq < mu && cmpNEq < di && cmpNEq < mi && cmpNEq < cmpEq && cmpNEq < cmpBi && cmpNEq < cmpSm) {
		newOp.pos = cmpNEq;
		memcpy(newOp.op, buffer + cmpNEq - 1, 2);
		newOp.op[2] = '\0';
		newOp.len = 2;
	} else if (cmpBi < pl && cmpBi < mu && cmpBi < di && cmpBi < mi && cmpBi < cmpEq && cmpBi < cmpNEq && cmpBi < cmpSm) {
		newOp.pos = cmpBi;
		memcpy(newOp.op, buffer + cmpBi, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	} else if (cmpSm < pl && cmpSm < mu && cmpSm < di && cmpSm < mi && cmpSm < cmpEq && cmpSm < cmpNEq && cmpSm < cmpBi) {
		newOp.pos = cmpSm;
		memcpy(newOp.op, buffer + cmpSm, 1);
		newOp.op[1] = '\0';
		newOp.len = 1;
	}
	return newOp;
}

bool Global::isTextString(char *cStr) {
	char *txtStart = strstr(cStr, "\"");
	if (txtStart) {
		char *txtEnd = strstr(txtStart + 1, "\"");
		if (txtEnd) {
			return true;
		}
	}
	return false;
}