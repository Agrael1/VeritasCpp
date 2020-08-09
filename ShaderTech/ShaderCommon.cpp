#include "ShaderCommon.h"
#include "SimpleVS.h"
#include "SimplePS.h"
#include <string_view>

HRESULT __stdcall VFMakeShader(const char* ShaderName, IVShader** _out_SPtr)
{
    std::string_view sShaderName{ ShaderName };
    if (sShaderName == "SimpleVS")
    {
        return wrl::MakeAndInitialize<SimpleVS>(_out_SPtr);
    }
    if (sShaderName == "SimplePS")
    {
        return wrl::MakeAndInitialize<SimplePS>(_out_SPtr);
    }
    return E_NOTIMPL;
}
