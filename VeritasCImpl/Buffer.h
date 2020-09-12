#pragma once
#include "Allocator.h"

#define c_class VBuffer

class
{
	COM_INTERFACE(IVBuffer);

	//Here goes data
	VBUFFER_DESC desc;
	uint8_t* data;
};

#if !defined(BUFFER_IMPL)
#undef c_class
#endif
