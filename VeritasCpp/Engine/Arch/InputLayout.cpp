#include "InputLayout.h"

InputLayout::InputLayout(VeritasEngine& gfx,
	std::span<VINPUT_ELEMENT> layout)
{
	GetDevice(gfx)->CreateInputLayout(layout.data(), layout.size(), nullptr, 0, &pInputLayout);
}

void InputLayout::Bind(VeritasEngine& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}