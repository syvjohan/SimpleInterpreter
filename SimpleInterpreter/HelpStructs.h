#pragma once

#include "Defines.h"

namespace Global {
	struct CallableUnit_s {
		char name[NAMESIZE];
		int startPos = -1;
		int endPos = -1;
	};

	struct Call_s {
		int pos;
		char name[NAMESIZE];
	};

	struct Loop_s {
		int start = -1;
		int end = -1;
		int stop = -1;
		int type = -1;
	};

	struct Alias_s {
		char name[NAMESIZE];
		char type[TYPESIZE];
		char value[VALUESIZE];
		int len;
	};

	struct Parts_s {
		char part[OUTPUTSIZE];
		int type; //1 = text. 2 = logic.
		int len;
	};

	struct Index_s {
		char name[NAMESIZE];
		char type[TYPESIZE];
		int startPos;
		int len;
	};

	struct Operator_s {
		int pos;
		char op[3];
		int len;
	};

	struct File_s {
		char name[PATHSIZE];
		int startPos = -1;
		int endPos = -1;
		int numberOfLines = -1;
	};
}