#pragma once
#include <Engine\Arch\Bindable.h>
#include <span>

class InputLayout : public Bindable
{
public:
	InputLayout(VeritasEngine& gfx, std::span<VINPUT_ELEMENT> layout);
public:
	void Bind(VeritasEngine& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<IVInputLayout> pInputLayout;
};

