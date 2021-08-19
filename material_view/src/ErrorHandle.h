#pragma once

#include "glad/glad.h"

#ifdef _DEBUG
#define IFDEBUG(X) X;\
void InitGLErrorHandling();
#else
#define IFDEBUG(X) do {} while (0)
#endif // _DEBUG
