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

extern void GetView(VTexture* This, VRTV_DESC* _out_rtv);

#if !defined(TEXTURE_IMPL)
#undef c_class
#endif
