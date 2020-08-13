#pragma once
#include <Engine\Arch\Bindable.h>
#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(VeritasEngine& gfx, const std::vector<VINPUT_ELEMENT>& layout);
	void Bind(VeritasEngine& gfx) noexcept override;
protected:
	Microsoft::WRL::ComPtr<IVInputLayout> pInputLayout;
};

