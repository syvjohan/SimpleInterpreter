#pragma once

#include "Trim.h"

#include <string.h>

#define NAMESIZE 50
#define ADDRESSSIZE 9
#define VALUESIZE 420
#define TYPESIZE 7
#define INSTRUCTIONSIZE 500

static int strCmp(const char *cStr1, const char *cStr2) {
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

static int checkForAlpha(const char *cStr) {
	int i = 0;
	while (cStr[i] != '\0') {
		if (!((cStr[i] >= 'a' && cStr[i] <= 'z') || (cStr[i] >= 'A' && cStr[i] <= 'Z'))) {
			return -1;
		}
		++i;
	}
	return 1;
}

static int checkForDigits(const char *cStr) {
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

struct Alias_s {
	char name[NAMESIZE];
	char type[TYPESIZE];
	char value[VALUESIZE];
	int len;
};

struct Index_s {
	char name[NAMESIZE];
	char type[TYPESIZE];
	int startPos;
	int len;
};
