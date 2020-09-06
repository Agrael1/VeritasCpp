#include "pch.h"
#include <string_view>
#include "PhongVS.h"
#include "PhongPS.h"

HRESULT __stdcall VFMakeShader(const char* ShaderName, IVShader** _out_SPtr)
{
    std::string_view sShaderName{ ShaderName };
    if (sShaderName == "PhongVS")
    {
        return wrl::MakeAndInitialize<PhongVS>(_out_SPtr);
    }
    if (sShaderName == "PhongPS")
    {
        return wrl::MakeAndInitialize<PhongPS>(_out_SPtr);
    }
	return E_NOTIMPL;
}
