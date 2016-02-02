#include "Global.h"
#include <cmath>

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