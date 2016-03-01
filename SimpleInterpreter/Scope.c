#include <stdio.h>
#include <stdlib.h>

typedef struct scope_s {
	int start;
	int end;
}scope_t;

scope_t *scope;
int len = 0;
int maxSize = 0;

int isEnd = 0;
int isStart = 0;

void incrementScope(int start, int end) {
	if (len == maxSize) {
		maxSize = ((maxSize * 2) + 20);
		scope = (scope_t *)malloc(maxSize * sizeof(scope_t));
	}

	if (isStart == 1 && isEnd == 1) {
		++len;
		isStart = 0;
		isEnd = 0;
	}

	if (end < 0) {
		scope[len].start = start;
		isStart = 1;

	} else if (start < 0) {
		scope[len].end = end;
		isEnd = 1;
	}
}

void decrementScope() {
	--len;
}

int getCurrentScopeStart() {
	return scope[len].start;
}

int getCurrentScopeEnd() {
	return scope[len].end;
}