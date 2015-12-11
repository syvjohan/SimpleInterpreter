#ifndef HEAP_H
#define HEAP_H

#include "Defs.h"
#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

const int nameSize = 30;

struct Index_s {
	char *name;
	void *type;
	int startPos;
	int len;
};

struct IStack {
		virtual void initializeStack(size_t stackSize) = 0;
		virtual int push(Index_s index, char *value) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual int pushAt(Index_s index, char *value) = 0; //returns 0 if not possible to insert value and 1 for success.
		virtual Alias_s getTop() = 0;
		virtual Alias_s getAt(int index) = 0;
		virtual void pop() = 0;

		int stackSize;
		int stackLen;
};

class Heap : IStack {
	public:
		Heap();
		~Heap();

		void initializeHeap(size_t heapSize);
		void insertAliasAt(int index, Alias_s alias);
		Alias_s getAlias(char *name);

		//Stack
		void initializeStack(size_t stackSize);
		int push(Index_s index, char *value);
		int pushAt(Index_s index, char *value);
		Alias_s getTop();
		Alias_s getAt(int index);
		void pop();

	private:
		void setHeapSize(size_t size);

		char *heapContainer;
		Index_s *heapIndex;
		size_t heapSize;
};

#endif //!HEAP_H