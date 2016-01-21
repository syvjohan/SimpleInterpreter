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

static char* strstrReverse(char *cStr, const char *searchedStr) {
	int lenCstr = strlen(cStr);
	int lenSearchedStr = strlen(searchedStr);
	int i, j = 0;
	int firstHit = -1;
	int flag = 0;

	if ((cStr == NULL || searchedStr == NULL)) return NULL;

	j = lenSearchedStr -1;
	for (i = lenCstr -1; i != 0; --i) {
		if (cStr[i] == searchedStr[j]) {
			if (j == 0) {
				flag = 1;
				break;
			}
			if (firstHit == -1) {
				firstHit = i +1;
			}
			--j;
		} else {
			j = lenSearchedStr - 1; //Cant identify the hole searchedStr without interuption, resetting counter.
			firstHit = -1;
		}
	}

	if (flag == 1) {
		char dest[INSTRUCTIONSIZE];
		memcpy(dest, cStr, firstHit);
		dest[firstHit] = '\0';
		return dest;
	}

	return NULL;
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
