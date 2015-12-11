#include "Stack.h"

Stack::Stack() {}

Stack::~Stack() {}

void Stack::init(int size) {
	index = DBG_NEW Index_s[size];
	this->size = size;
	len = 0;
}

int Stack::push(Index_s index, char *value) {

}

char* Stack::getTop() {

}

char* Stack::getAt(int index) {

}

void Stack::pop() {

}
