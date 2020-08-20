#pragma once
#include "Bindable.h"
#include <string_view>

class PixelShader : public Bindable
{
public:
	PixelShader(std::string_view shaderName)
	{
		VFMakeShader(shaderName.data(), &pPS);
	}
	void Bind(VeritasEngine& vin)override
	{
		GetContext(vin)->PSSetShader(pPS.Get());
	}
private:
	wrl::ComPtr<IVShader> pPS;
};