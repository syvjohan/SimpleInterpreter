#pragma once

namespace Global {
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

	static void trimWhitespacesExceptInTextString(char *cStr) {
		int insideTextString = 0;
		int i = 0;
		int j = 0;
		while (cStr[i] != '\0') {
			if (cStr[i] == '\"') {
				if (insideTextString == 0) {
					insideTextString = 1;
				} else {
					insideTextString = 0;
				}
			}

			if (cStr[i] != ' ' && insideTextString != 1) {
				cStr[j++] = cStr[i];
			} else if (insideTextString == 1) {
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

	static void trimAnd(char *cStr) {
		int i = 0;
		int j = 0;
		while (cStr[i] != '\0') {
			if (cStr[i] != '&') {
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
}