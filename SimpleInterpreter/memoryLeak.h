#ifndef MEMORYLEAK_H
#define MEMORYLEAK_H

#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new(1, __FILE__, __LINE__)
#else
#define DBG_NEW new
#endif


#endif //!MEMORYLEAK_H