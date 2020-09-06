#pragma once
#include <Interfaces.h>
#include <array>

class VInputLayout : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVInputLayout>
{
    friend class InputAssembler;
public:
    HRESULT RuntimeClassInitialize(const VINPUT_ELEMENT* in_descs, uint32_t NumElements);
public:
    VInputLayout() = default;
private:
    std::array<VINPUT_ELEMENT, 16> il;
    uint32_t monotonicSize = 0;
};