#ifndef HEAP_H
#define HEAP_H

#include "Defs.h"
#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define EXPRESSIONSIZE 100

struct Index_s {
	char *name;
	char *type;
	int startPos;
	int len;
};

struct IStack {
		virtual void createStack(size_t stackSize) = 0;
		virtual int pushTop(Alias_s alias) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual int pushAt(int index, Alias_s alias) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual Alias_s getTop() = 0;
		virtual Alias_s getAt(int index) = 0;
		virtual void pop() = 0;

		int stackSize = -1;
		int stackLen = -1;
};

class Heap : IStack {
	public:
		Heap();
		~Heap();

		void initializeHeap(size_t heapSize);
		int insertAliasAt(int index, Alias_s alias);
		Alias_s getAlias(char *name);
		Alias_s getAlias(int index);
		int getAddress(char *name);

		//Stack
		void createStack(size_t stackSize);
		int pushTop(Alias_s alias);
		int pushAt(int index, Alias_s alias);
		Alias_s getTop();
		Alias_s getAt(int index);
		void pop();

	private:
		void setHeapSize(size_t size);

		char *heapContainer;
		Index_s *heapIndex;
		size_t heapStartPos;
		size_t heapSize;

		char tmpStr[EXPRESSIONSIZE];
};

#endif //!HEAP_H