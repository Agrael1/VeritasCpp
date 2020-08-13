#pragma once
#include <Interfaces.h>
#include <vector>
#include <algorithm>

class VBuffer : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVBuffer>
{
    friend class InputAssembler;
public:
    HRESULT RuntimeClassInitialize(const VBUFFER_DESC* in_desc, const void* _in_data);
public:
    VBuffer() = default;
public:
    virtual void __stdcall GetDesc(VBUFFER_DESC* _out_Desc)override;
private:
    VBUFFER_DESC desc{ 0 };
    std::vector<uint8_t> data;
};
