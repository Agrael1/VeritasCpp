#include "Bindable.h"

class DepthStencil : public Bindable
{
public:
	DepthStencil(VeritasEngine& in, uint32_t width, uint32_t height);
public:
	void Bind(VeritasEngine& in)override;
private:
	wrl::ComPtr<IVTexture> pDS;
	VDSV_DESC DSV;
};