#include "InputLayout.h"

HRESULT VInputLayout::RuntimeClassInitialize(const VINPUT_ELEMENT* in_descs, uint32_t NumElements)
{
    if (in_descs) std::copy(in_descs, in_descs + NumElements, il.begin());
    return S_OK;
}
