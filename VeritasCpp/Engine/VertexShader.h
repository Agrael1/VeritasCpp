#pragma once
#include "Bindable.h"
#include <string_view>

class VertexShader : public Bindable
{
public:
	VertexShader(std::string_view shadertag);
public:
	void Bind(class VeritasEngine& in)override;
private:
	wrl::ComPtr<IVShader> vs;
};