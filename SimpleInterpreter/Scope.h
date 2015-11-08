#ifndef SCOPE_H
#define SCOPE_H

void incrementScope(int start, int end);
void decrementScope();
char* getCurrentScopeStart();
char* getCurrentScopeEnd();

#endif //!SCOPE_H