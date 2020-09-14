#define INPUTLAYOUT_IMPL
#include "InputLayout.h"

HRESULT RuntimeClassInitialize(VInputLayout* This, const VINPUT_ELEMENT* in_descs, uint32_t NumElements)
{
	if (in_descs) memcpy_s(This->il, 16 * sizeof(VINPUT_ELEMENT), in_descs, NumElements * sizeof(VINPUT_ELEMENT));
	This->monotonicSize = NumElements;
	return S_OK;
}
void RuntimeClassDestroy(VInputLayout* This)
{

}
VirtualTable(IVInputLayout)
{
	._unknwn = __COM_IUnknown,
};
ENDCLASSDESC