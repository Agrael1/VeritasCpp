#pragma once
#include "Allocator.h"

#define c_class VGraphicsDevice

class
{
	COM_INTERFACE(IVDevice);
};

#if !defined(DEVICE_IMPL)
#undef c_class
#endif