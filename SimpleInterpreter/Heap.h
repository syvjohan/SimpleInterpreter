#ifndef HEAP_H
#define HEAP_H

#include "IStack.h"
#include "Defines.h"

struct Alias_s;
struct Index_s;

class Heap : public IStack {
	public:
		Heap();
		~Heap();

		//Heap
		void InitializeHeap(const size_t heapSize);
		void InsertAliasAt(const int index, const Alias_s alias);
		Alias_s GetAlias(const char *name);
		Alias_s GetAlias(const int index);
		int GetAddress(const char *name);
		void UpdateHeapIndex(const Index_s index);

		//Stack
		void CreateStack(const size_t stackSize); 
		void PushTop(const Alias_s alias);
		void PushAt(const int index, const Alias_s alias);
		Alias_s GetTop();
		Alias_s GetAt(const int index);
		void PopTop();
		void Pop();
		bool IsStackOverflow(const int index, const int len);

		//Index
		//Below functions is specialcases for content inside struct
		void InsertStructIndex(const Index_s index);
		bool UpdateStructIndex(const Index_s index, const char *searchName);
		bool FindStructIndex(const Index_s index);
		Index_s FindStructIndex(const char *name);
		Index_s GetStructIndex(const char *name);
		void GetStructIndex(const char *type, Index_s *index, int &len);
		void TypedefStructMembers(const char *searchName, const char *extendName);
		void UpdateStructHeaderPointer(const Index_s index);
		char* GetFullNameStructMember(char *lastname);

	private:
		void SetHeapSize(size_t size);
		void InsertAt(const int index, const Alias_s alias);

		char *heapContainer;
		Index_s *heapIndex;
		size_t heapStartPos;
		size_t heapSize;
		
		char tmpStr[INSTRUCTIONSIZE];

		//Index
		//Specialcases for content inside struct
		void ExpandHeapIndexStruct();
		Index_s *heapIndexStructs;
		size_t indexStructLen = 0;
		size_t indexStructMax = 0;
};

#endif //!HEAP_H