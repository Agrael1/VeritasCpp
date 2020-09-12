#pragma once
#include "Allocator.h"

#define c_class VTexture

class
{
	COM_INTERFACE(IVTexture);

	//Here goes your data
	VTEXTURE_DESC desc;
	uint32_t size;
	uint8_t* data;
};

#if !defined(TEXTURE_IMPL)
#undef c_class
#endif
