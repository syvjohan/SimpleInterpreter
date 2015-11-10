#ifndef SCOPE_H
#define SCOPE_H

void incrementScope(int start, int end);
void decrementScope();
int getCurrentScopeStart();
int getCurrentScopeEnd();

#endif //!SCOPE_H