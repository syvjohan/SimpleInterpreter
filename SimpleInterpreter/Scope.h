#ifndef SCOPE_H
#define SCOPE_H

#include <stdio.h>
#include <stdlib.h>

struct Scope {
	struct Index_s {
		int start;
		int end;
	};

	Index_s *arrIndex;
	int len = 0;
	int maxSize = 0;
	int isEnd = 0;
	int isStart = 0;

	void incrementScope(int start, int end) {
		if (len == maxSize) {
			maxSize = ((maxSize * 2) + 20);
			arrIndex = (Index_s *)malloc(maxSize * sizeof(Index_s));
		}

		if (isStart == 1 && isEnd == 1) {
			++len;
			isStart = 0;
			isEnd = 0;
		}

		if (end < 0) {
			arrIndex[len].start = start;
			isStart = 1;

		} else if (start < 0) {
			arrIndex[len].end = end;
			isEnd = 1;
		}
	}

	void decrementScope() {
		--len;
	}

	int getCurrentScopeStart() {
		return arrIndex[len].start;
	}

	int getCurrentScopeEnd() {
		return arrIndex[len].end;
	}
};

#endif //!SCOPE_H