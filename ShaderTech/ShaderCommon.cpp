#include "ShaderCommon.h"
#include "SimpleVS.h"
#include <string_view>

HRESULT __stdcall VFMakeShader(const char* ShaderName, IVVertexShader** _out_VSPtr)
{
    if (std::string_view(ShaderName) == "SimpleVS")
    {
        return wrl::MakeAndInitialize<SimpleVS>(_out_VSPtr);
    }
    return E_NOTIMPL;
}
