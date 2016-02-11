#ifndef HEAP_H
#define HEAP_H

#include "IStack.h"
#include "Defines.h"
#include "Global.h"
#include "ErrorManager.h"

struct Alias_s;
struct Index_s;

class Heap : public IStack {
	public:
		Heap();
		~Heap();

		//Heap
		void initializeHeap(size_t heapSize);
		int insertAliasAt(int index, Alias_s alias);
		Alias_s getAlias(const char *name);
		Alias_s getAlias(const int index);
		int getAddress(char *name);
		void updateHeapIndex(Index_s index);

		//Stack
		void createStack(size_t stackSize); 
		int pushTop(Alias_s alias);
		int pushAt(int index, Alias_s alias);
		Alias_s getTop();
		Alias_s getAt(const int index);
		void popTop();
		void pop();

		//Index
		//Below functions is specialcases for content inside struct
		void insertStructIndex(Index_s index);
		bool updateStructIndex(Index_s index, char *searchName);
		bool findStructIndex(Index_s index);
		Index_s findStructIndex(char *name);
		Index_s getStructIndex(const char *name);
		void getStructIndex(const char *type, Index_s *index, int &len);
		void typedefStructMembers(char *searchName, char *extendName);
		void updateStructHeaderPointer(Index_s index);
		char* getFullNameStructMember(char *lastname);

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
		ErrorManager errorManager;
};

#endif //!HEAP_H