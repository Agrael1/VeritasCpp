#pragma once
#include "Allocator.h"

#define c_class VInputLayout

class
{
    COM_INTERFACE(IVInputLayout);

    VINPUT_ELEMENT il[16];
    uint32_t monotonicSize;
};

#if !defined(INPUTLAYOUT_IMPL)
#undef c_class
#endif