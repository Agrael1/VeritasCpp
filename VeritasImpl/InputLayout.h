#pragma once
#include <Interfaces.h>
#include <vector>
#include <algorithm>

class VInputLayout : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVInputLayout>
{
    friend class InputAssembler;
public:
    HRESULT RuntimeClassInitialize(const VINPUT_ELEMENT* in_descs, uint32_t NumElements);
public:
    VInputLayout() = default;
private:
    std::vector<VINPUT_ELEMENT> il;
};
