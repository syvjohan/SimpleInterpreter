#pragma once

#include "Defs.h"



class Stack {
	public:
		Stack();
		~Stack();
		void init(int size);
		int push(Index_s index, char *value); //returns 0 if not possible to insert value and 1 for success.
		char* getTop();
		char* getAt(int index);
		void pop();

	private:
		
};

