#pragma once

//Call by reference changes the passed argument no need for returning a value.

static void trimWhitespaces(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimBothParanthesis(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ')' && cStr[i] != '(') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimCloseParanthes(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ')') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimOpenParanthes(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != '(') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimHeap(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ' ' && cStr[i] != '&' && cStr[i] != '#' && cStr[i] != ')' && cStr[i] != '(' && cStr[i] != ';') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimText(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != '\"') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimNewline(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != '\n') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimTabb(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != '\t') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimBracket(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != '{' && cStr[i] != '}') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}

static void trimSemicolon(char *cStr) {
	int i = 0;
	int j = 0;
	while (cStr[i] != '\0') {
		if (cStr[i] != ';') {
			cStr[j++] = cStr[i];
		}
		i++;
	}
	cStr[j] = '\0';
}