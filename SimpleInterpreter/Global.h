#pragma once

#include <string.h>

static int strCmp(const char *cStr1, const char *cStr2) {
	int i = 0;
	if (strlen(cStr1) <= 0 || strlen(cStr2) <= 0) { return 0; }
	while (cStr1[i] != '\0' || cStr2[i] != '\0') {
		if (cStr1[i] != cStr2[i]) {
			return 0;
		}
		++i;
	}
	return 1;
}