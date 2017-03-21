#pragma once

namespace Global {
	struct Alias_s;
}

namespace Memory {
	class IStack {
	public:
		IStack() {};
		virtual ~IStack() {};

		virtual void CreateStack( const size_t stackSize ) = 0;
		virtual void PushTop( const Global::Alias_s alias ) = 0;
		virtual void PushAt( const int index, const Global::Alias_s alias ) = 0;
		virtual Global::Alias_s GetTop() = 0;
		virtual Global::Alias_s GetAt( const int index ) = 0;
		virtual void PopTop() = 0;
		virtual void Pop() = 0;

		int GetStackSize() { return stackSize; };
		void SetStackSize( int size ) { stackSize = size; };
		int GetStackLen() { return stackLen; };
		void SetStackLen( int len ) { stackLen = len; };

	private:
		int stackSize = -1;
		int stackLen = -1;
	};
}
