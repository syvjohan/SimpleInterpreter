#ifndef HEAP_H
#define HEAP_H

#include "IStack.h"
#include "Defines.h"
#include "Global.h"

struct Alias_s;
struct Index_s;

class Heap : public IStack {
	public:
		Heap();
		~Heap();

		//Heap
		void initializeHeap(size_t heapSize);
		int insertAliasAt(int index, Alias_s alias);
		Alias_s getAlias(char *name);
		Alias_s getAlias(int index);
		int getAddress(char *name);
		void updateHeapIndex(Index_s index);

		//Stack
		void createStack(size_t stackSize); 
		int pushTop(Alias_s alias);
		int pushAt(int index, Alias_s alias);
		Alias_s getTop();
		Alias_s getAt(const int index);
		void pop();

		//Index
		//Below functions is specialcases for content inside struct
		void insertStructIndex(Index_s index);
		bool updateStructIndex(Index_s index);
		Index_s getStructIndex(char *name);

	private:
		void setHeapSize(size_t size);

		char *heapContainer;
		Index_s *heapIndex;
		size_t heapStartPos;
		size_t heapSize;
		
		char tmpStr[INSTRUCTIONSIZE];

		//Index
		//Specialcases for content inside struct
		void expandHeapIndexStruct();
		Index_s *heapIndexStructs;
		size_t indexStructLen = 0;
		size_t indexStructMax = 0;

		Global global;
};

#endif //!HEAP_H