#ifndef HEAP_H
#define HEAP_H

#include "IStack.h"
#include "Defines.h"

namespace Global {
	struct Alias_s;
	struct Index_s;
}

namespace Memory {
	class Heap : public Memory::IStack {
	public:
		Heap();
		~Heap();

		//Heap
		void InitializeHeap( const size_t heapSize );
		void InsertAliasAt( const int index, const Global::Alias_s alias );
		Global::Alias_s GetAlias( const char *name );
		Global::Alias_s GetAlias( const int index );
		const char GetValueAt( const int memoryAddress );
		int GetAddress( const char *name );
		void UpdateHeapIndex( const Global::Index_s index );

		//Stack
		void CreateStack( const size_t stackSize );
		void PushTop( const Global::Alias_s alias );
		void PushAt( const int index, const Global::Alias_s alias );
		Global::Alias_s GetTop();
		Global::Alias_s GetAt( const int index );
		Global::Alias_s GetSize();
		void PopTop();
		void Pop();
		bool IsStackOverflow( const int index, const int len );

		//Index
		//Below functions is specialcases for content inside struct
		void InsertStructIndex( const Global::Index_s index );
		bool UpdateStructIndex( const Global::Index_s index, const char *searchName );
		bool FindStructIndex( const Global::Index_s index );
		Global::Index_s FindStructIndex( const char *name );
		Global::Index_s GetStructIndex( const char *name );
		void GetStructIndex( const char *type, Global::Index_s *index, int &len );
		void TypedefStructMembers( const char *searchName, const char *extendName );
		void UpdateStructHeaderPointer( const Global::Index_s index );
		char* GetFullNameStructMember( const char *lastname );

	private:
		void SetHeapSize( size_t size );
		void InsertAt( const int index, const Global::Alias_s alias );

		char *heapContainer;
		Global::Index_s *heapIndex;
		size_t heapStartPos;
		size_t heapSize;

		char tmpStr[ INSTRUCTIONSIZE ];

		//Index
		//Specialcases for content inside struct
		void ExpandHeapIndexStruct();
		Global::Index_s *heapIndexStructs;
		size_t indexStructLen = 0;
		size_t indexStructMax = 0;
	};
}
#endif //!HEAP_H