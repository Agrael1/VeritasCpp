#include "RuntimeClass.h"
#include "Buffer.h"

#pragma region COM Routines
void MakeBuffer(void** x)
{
	MakeAndInitialize(x, VBuffer);
}


#pragma endregion
