#include "Context.h"

#undef min
#undef max

HRESULT __stdcall VContext::IASetPrimitiveTopology(VPRIMITIVE_TOPOLOGY Topology)
{
	//IATopology = Topology;
	return S_OK;
}
HRESULT __stdcall VContext::IASetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
{
	return IAStage.SetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}
HRESULT __stdcall VContext::IASetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	return IAStage.SetIndexBuffer(indexBuffer, format, offsetBytes);
}
HRESULT __stdcall VContext::IASetInputLayout(IVInputLayout* pInputLayout)
{
	return IAStage.SetInputLayout(pInputLayout);
}

HRESULT __stdcall VContext::VSSetShader(IVShader* pVertexShader)
{
	VSVertexShader = pVertexShader;
	return S_OK;
}
HRESULT __stdcall VContext::VSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	std::span<VBuffer* const> x{ (VBuffer* const*)ppConstantBuffers, NumBuffers };
	std::copy(x.begin(), x.end(), VSConstantBuffers.begin() + StartSlot);
	return S_OK;
}
HRESULT __stdcall VContext::PSSetShader(IVShader* pPixelShader)
{
	PSPixelShader = pPixelShader;
	return S_OK;
}
HRESULT __stdcall VContext::PSSetConstantBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	std::span<VBuffer* const> x{ (VBuffer* const*)ppConstantBuffers, NumBuffers };
	std::copy(x.begin(), x.end(), PSConstantBuffers.begin() + StartSlot);
	return S_OK;
}
HRESULT __stdcall VContext::RSSetViewport(uint32_t numVPs, const VVIEWPORT_DESC* in)
{
	RSViewPort = *in;
	const float HalfViewportWidth = RSViewPort.Width * 0.5f;
	const float HalfViewportHeight = RSViewPort.Height * 0.5f;
	RSVPScale = dx::XMVectorSet(HalfViewportWidth, -HalfViewportHeight, 1.0f, 0.0f);
	RSVPOffset = dx::XMVectorSet(HalfViewportWidth, HalfViewportHeight, 1.0f, 0.0f);
	return S_OK;
}
HRESULT __stdcall VContext::OMSetRenderTargets(uint32_t numViews, const VRTV_DESC* const _arr_RTVs)
{
	if (numViews > MaxRenderTargets) return E_INVALIDARG;
	std::copy_n(_arr_RTVs, numViews, OMRenderTargets.begin());
	return S_OK;
}
HRESULT __stdcall VContext::OMSetDepthStencil(const VDSV_DESC* DSV)
{
	if (DSV)
		OMRenderDepth = *DSV;
	else
		OMRenderDepth.Scan0 = nullptr;
	return S_OK;
}
HRESULT __stdcall VContext::ClearRenderTarget(VRTV_DESC* rtv, uint32_t col)
{
	std::fill((uint32_t*)rtv->Scan0, (uint32_t*)rtv->Scan0 + rtv->Width * rtv->Height, col);
	return S_OK;
}
HRESULT __stdcall VContext::ClearDepthStencil(VDSV_DESC* dsv, float value)
{
	float x = value;
	std::fill((float*)dsv->Scan0, (float*)dsv->Scan0 + size_t(dsv->Width) * dsv->Height, x);
	return S_OK;
}

HRESULT __stdcall VContext::Map(IVBuffer* pResource, VMAPPED_SUBRESOURCE* _out_pMappedResource)
{
	auto* x = dynamic_cast<VBuffer*>(pResource);
	if (!x) return E_POINTER;

	_out_pMappedResource->pData = x->data.data();
	_out_pMappedResource->RowPitch = x->desc.StructureByteStride;
	_out_pMappedResource->DepthPitch = x->desc.ByteWidth;

	return S_OK;
}
HRESULT __stdcall VContext::Unmap(IVBuffer* pResource)
{
	// may be used for multithreading for unlocking buffer for gpu thread 
	return E_NOTIMPL;
}

void __stdcall VContext::DrawIndexed(uint32_t nofVertices)
{
	auto verts = IAStage.MakeVerticesIndexed(nofVertices);

	std::array<void*, 4> x{};
	for (size_t i = 0; i < 4; i++)
	{
		x[i] = VSConstantBuffers[i] ? VSConstantBuffers[i]->data.data() : nullptr;
	}
	VSVertexShader->UpdateConstants(x.data());

	for (size_t i = 0; i < 4; i++)
	{
		x[i] = PSConstantBuffers[i] ? PSConstantBuffers[i]->data.data() : nullptr;
	}
	PSPixelShader->UpdateConstants(x.data());

	std::vector<XMVSOut> VSOut;
	VSOut.resize(verts.size());

	for (uint32_t i = 0; auto & v : verts)
	{
		VSVertexShader->Invoke(&v, &VSOut[i++]);
	}
	AssembleTriangles(VSOut);
}
void VContext::AssembleTriangles(std::vector<XMVSOut>& VSOut)
{
	ShaderPrivateData pd;
	VSVertexShader->GetShaderPrivateData(&pd);

	for (size_t it = 0u; it < VSOut.size(); it += 3)
	{
		auto& v0 = VSOut[it];
		auto& v1 = VSOut[it + 1];
		auto& v2 = VSOut[it + 2];

		RSClipCullTriangles(v0, v1, v2, pd);
	}
}

void VContext::RSClipCullTriangles(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2, ShaderPrivateData pd)
{
	constexpr dx::XMVECTORU32 preor{ 0x80000000, 0x80000000, 0x00000000, 0x00000000 };
	constexpr dx::XMVECTORU32 preand{ 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF };

	using namespace DirectX;
	const auto Clip1V = [=](XMVSOut& v0, XMVSOut& v1, XMVSOut& v2)
	{
		const float alphaA = (-v0[pd.PositionIndex].z) / (v1[pd.PositionIndex].z - v0[pd.PositionIndex].z);
		const float alphaB = (-v0[pd.PositionIndex].z) / (v2[pd.PositionIndex].z - v0[pd.PositionIndex].z);
		// interpolate to get v0a and v0b
		auto v0a = VSOutInterpolate(v0, v1, alphaA, pd.VertexSize);
		auto v0b = VSOutInterpolate(v0, v2, alphaB, pd.VertexSize);
		auto v0a2 = v0a;
		auto v2b = v2;
		// draw triangles
		RSPostProcessTriangle(v0a, v1, v2, pd);
		RSPostProcessTriangle(v0b, v0a2, v2b, pd);
	};
	const auto Clip2V = [=](XMVSOut& v0, XMVSOut& v1, XMVSOut& v2)
	{
		// calculate alpha values for getting adjusted vertices
		const float alpha0 = (-v0[pd.PositionIndex].z) / (v2[pd.PositionIndex].z - v0[pd.PositionIndex].z);
		const float alpha1 = (-v1[pd.PositionIndex].z) / (v2[pd.PositionIndex].z - v1[pd.PositionIndex].z);
		// interpolate to get v0a and v0b

		v0 = VSOutInterpolate(v0, v2, alpha0, pd.VertexSize);
		v1 = VSOutInterpolate(v1, v2, alpha1, pd.VertexSize);
		// draw triangles
		RSPostProcessTriangle(v0, v1, v2, pd);
	};

	XMVECTOR V0 = v0[pd.PositionIndex];
	XMVECTOR V1 = v1[pd.PositionIndex];
	XMVECTOR V2 = v2[pd.PositionIndex];

	// Compare againgst W value
	XMVECTOR CT0 = XMVectorSplatW(V0);
	XMVECTOR CT1 = XMVectorSplatW(V1);
	XMVECTOR CT2 = XMVectorSplatW(V2);

	XMVECTOR R01 = XMVectorLess(CT0, V0);
	XMVECTOR R11 = XMVectorLess(CT1, V1);
	XMVECTOR R21 = XMVectorLess(CT2, V2);

	XMVECTOR RR1 = XMVectorAndInt(XMVectorAndInt(R01, R11), R21);
	if (_mm_movemask_ps(RR1) != 0) return;

	CT0 = _mm_and_ps(_mm_or_ps(CT0, preor), preand);
	CT1 = _mm_and_ps(_mm_or_ps(CT1, preor), preand);
	CT2 = _mm_and_ps(_mm_or_ps(CT2, preor), preand);

	XMVECTOR R02 = XMVectorLess(V0, CT0);
	XMVECTOR R12 = XMVectorLess(V1, CT1);
	XMVECTOR R22 = XMVectorLess(V2, CT2);
	XMVECTOR RR2 = XMVectorAndInt(XMVectorAndInt(R02, R12), R22);
	if (_mm_movemask_ps(RR2) != 0) return;

	RR1 = XMVectorMergeZW(R02, R12);
	RR2 = _mm_shuffle_ps(RR1, R22, _MM_SHUFFLE(3, 2, 1, 0));

	int selector = _mm_movemask_ps(RR2) & 7;
	switch (selector)
	{
	case 0: RSPostProcessTriangle(v0, v1, v2, pd); break;
	case 1: Clip1V(v0, v1, v2); break;
	case 2: Clip1V(v1, v2, v0); break;
	case 3: Clip2V(v0, v1, v2); break;
	case 4: Clip1V(v2, v0, v1); break;
	case 5: Clip2V(v2, v0, v1); break;
	case 6: Clip2V(v1, v2, v0); break;
	}
}
void VContext::RSPostProcessTriangle(XMVSOut& v0, XMVSOut& v1, XMVSOut& v2, ShaderPrivateData pd)
{
	using namespace DirectX;
	auto idx = pd.PositionIndex;
	// homo -> NDC space transformation
	XMVECTOR wInv = XMVectorReciprocal(_mm_shuffle_ps(XMVectorMergeZW(v0[idx], v1[idx]), v2[idx], _MM_SHUFFLE(3, 3, 3, 2)));
	XMMATRIX X;
	X.r[0] = XMVectorSplatX(wInv); // 1/w0
	X.r[1] = XMVectorSplatY(wInv); // 1/w1
	X.r[2] = XMVectorSplatZ(wInv); // 1/w2

								   //Screen space transform, 1/w is stored in W
	v0[idx] = XMVectorMultiplyAdd(v0[idx] * X.r[0], RSVPScale, RSVPOffset); XMStoreFloat(&v0[idx].w, X.r[0]);
	v1[idx] = XMVectorMultiplyAdd(v1[idx] * X.r[1], RSVPScale, RSVPOffset); XMStoreFloat(&v1[idx].w, X.r[1]);
	v2[idx] = XMVectorMultiplyAdd(v2[idx] * X.r[2], RSVPScale, RSVPOffset); XMStoreFloat(&v2[idx].w, X.r[2]);

	//cull backfaces
	if (float4(XMVector3Cross((v1[idx] - v0[idx]), (v2[idx] - v0[idx]))).z < 0.0f)
		return;

	for (unsigned i = 0; i < pd.VertexSize; i++)
	{
		if (i == idx) continue;
		v0[i] = v0[i] * X.r[0];
		v1[i] = v1[i] * X.r[1];
		v2[i] = v2[i] * X.r[2];
	}
	DrawTriangle(v0, v1, v2, pd);
}

void VContext::DrawTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, ShaderPrivateData pd)
{
	uint32_t idx = pd.PositionIndex;
	// using pointers so we can swap (for sorting purposes)
	const auto* pv0 = &Vo0;
	const auto* pv1 = &Vo1;
	const auto* pv2 = &Vo2;

	// sorting vertices by y
	if (pv1->attributes[idx].y < pv0->attributes[idx].y) std::swap(pv0, pv1);
	if (pv2->attributes[idx].y < pv1->attributes[idx].y) std::swap(pv1, pv2);
	if (pv1->attributes[idx].y < pv0->attributes[idx].y) std::swap(pv0, pv1);

	if (pv0->attributes[idx].y == pv1->attributes[idx].y) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->attributes[idx].x < pv0->attributes[idx].x) std::swap(pv0, pv1);

		DrawFlatTopTriangle(*pv0, *pv1, *pv2, pd);
	}
	else if (pv1->attributes[idx].y == pv2->attributes[idx].y) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->attributes[idx].x < pv1->attributes[idx].x) std::swap(pv1, pv2);

		DrawFlatBottomTriangle(*pv0, *pv1, *pv2, pd);
	}
	else // general triangle
	{
		// find splitting vertex interpolant
		const float alphaSplit =
			(pv1->attributes[idx].y - pv0->attributes[idx].y) /
			(pv2->attributes[idx].y - pv0->attributes[idx].y);
		const auto vi = VSOutInterpolate(*pv0, *pv2, alphaSplit, pd.VertexSize);

		if (pv1->attributes[idx].x < vi.attributes[idx].x) // major right
		{
			DrawFlatBottomTriangle(*pv0, *pv1, vi, pd);
			DrawFlatTopTriangle(*pv1, vi, *pv2, pd);
		}
		else // major left
		{
			DrawFlatBottomTriangle(*pv0, vi, *pv1, pd);
			DrawFlatTopTriangle(vi, *pv1, *pv2, pd);
		}
	}
}
void VContext::DrawFlatTopTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, ShaderPrivateData pd)
{
	using namespace DirectX;
	XMVSOut InterpolantEdge = Vo1;
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	auto dv0 = XMVSOut::Subtract(Vo2, Vo0, pd.VertexSize);
	auto dv1 = XMVSOut::Subtract(Vo2, Vo1, pd.VertexSize);
	FXMVECTOR delta_Y = XMVectorSplatY(XMVectorReciprocal(dv0[pd.PositionIndex]));

	// delta over 0 and 1 resp
	dv0.Scale(delta_Y, pd.VertexSize);
	dv1.Scale(delta_Y, pd.VertexSize);

	// call the flat triangle render routine, right edge interpolant is it1
	DrawFlatTriangle(Vo0, Vo2, dv0, dv1, InterpolantEdge, pd);
}
void VContext::DrawFlatBottomTriangle(const XMVSOut& Vo0, const XMVSOut& Vo1, const XMVSOut& Vo2, ShaderPrivateData pd)
{
	using namespace DirectX;
	XMVSOut InterpolantEdge = Vo0;

	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	auto dv0 = XMVSOut::Subtract(Vo1, Vo0, pd.VertexSize);
	auto dv1 = XMVSOut::Subtract(Vo2, Vo0, pd.VertexSize);
	FXMVECTOR delta_Y = XMVectorReciprocal(XMVectorSplatY(dv0[pd.PositionIndex])); // minimize div (reciprocalEst is not good enough)

																			// delta over 1 and 2 resp
	dv0.Scale(delta_Y, pd.VertexSize);
	dv1.Scale(delta_Y, pd.VertexSize);

	// call the flat triangle render routine, right edge interpolant is it0
	DrawFlatTriangle(Vo0, Vo2, dv0, dv1, InterpolantEdge, pd);
}
void VContext::DrawFlatTriangle(const XMVSOut& it0, const XMVSOut& it2, const XMVSOut& dv0, const XMVSOut& dv1, XMVSOut& itEdge1, ShaderPrivateData pd)
{
	using namespace DirectX;

	// create edge interpolant for left edge (always v0)
	XMVSOut itEdge0 = it0;
	XMVSOut iLine;
	XMVSOut diLine;
	XMVSOut _P;
	auto idx = pd.PositionIndex;

	// calculate start and end scanlines (AABB)
	const int yStart = std::max((int)ceil(it0[idx].y - 0.5f), 0);
	const int yEnd = std::min((int)ceil(it2[idx].y - 0.5f), (int)RSViewPort.Height); // the scanline AFTER the last line drawn

																						   // do interpolant prestep
	FXMVECTOR step = XMVectorReplicate(((float)yStart + 0.5f - it0[idx].y));

	for (unsigned i = 0; i < pd.VertexSize; i++)
	{
		itEdge0[i] = XMVectorMultiplyAdd(dv0[i], step, itEdge0[i]);
		itEdge1[i] = XMVectorMultiplyAdd(dv1[i], step, itEdge1[i]);
	}

	for (int y = yStart; y < yEnd; y++, itEdge0.Increase(dv0, pd.VertexSize), itEdge1.Increase(dv1, pd.VertexSize))
	{
		// calculate start and end pixels
		const int xStart = std::max((int)ceil(itEdge0[idx].x - 0.5f), 0);
		const int xEnd = std::min((int)ceil(itEdge1[idx].x - 0.5f), (int)RSViewPort.Width); // the pixel AFTER the last pixel drawn

																								  // create scanline interpolant startpoint
																								  // (some waste for interpolating x,y,z, but makes life easier not having
																								  //  to split them off, and z will be needed in the future anyways...)

		iLine = itEdge0;
		FXMVECTOR step2 = XMVectorReplicate((float)xStart + 0.5f - itEdge0[idx].x);
		FXMVECTOR Delta_X = XMVectorReciprocal(XMVectorSplatX(itEdge1[idx] - itEdge0[idx]));
		diLine = XMVSOut::Multiply(XMVSOut::Subtract(itEdge1, iLine, pd.VertexSize), Delta_X, pd.VertexSize);
		iLine.Increase(XMVSOut::Multiply(diLine, step2, pd.VertexSize), pd.VertexSize);

		const size_t premulI = y * size_t(RSViewPort.Width);


		for (int x = xStart; x < xEnd; x++, iLine.Increase(diLine, pd.VertexSize))
		{
			if (auto [pass, zv] = DepthTest(x, premulI, iLine[idx].z); pass)
			{
				// recover interpolated z from interpolated 1/z
				FXMVECTOR w = XMVectorReciprocalEst(XMVectorSplatW(iLine[idx]));
				for (unsigned i = 0; i < pd.VertexSize; i++)
					_P[i] = iLine[i] * w;
				// invoke pixel shader with interpolated vertex attributes
				// and use result to set the pixel color on the screen
				if (PSPixelShader)
				{
					PackedVector::XMCOLOR col;
					PSPixelShader->Invoke(&_P, &col);

					if (zv == ((float*)OMRenderDepth.Scan0)[premulI + x]) //afxmt sanity check for data races
					{
						((uint32_t*)OMRenderTargets[0].Scan0)[premulI + x] = col;
					}
				}
			}
		}
	}
}
std::pair<bool, float> VContext::DepthTest(uint32_t width_in, size_t PremulIndex, float z)
{
	//Warning! lockless programming!
	auto* reg = reinterpret_cast<std::atomic<float>*>(&((float*)OMRenderDepth.Scan0)[PremulIndex + width_in]);
	auto zv = std::atomic_load(reg);

	do
	{
		if (z >= zv) return { false, z };
	} while (!std::atomic_compare_exchange_weak(reg, &zv, z));

	return { true, z };
}

HRESULT InputAssembler::SetIndexBuffer(IVBuffer* indexBuffer, VFORMAT format, uint32_t offsetBytes)
{
	IAIndexFormat = format;
	if (format)
	{
		IAIndexBuffer = dynamic_cast<VBuffer*>(indexBuffer);
		IAIndexOffset = offsetBytes;
	}
	else
	{
		IAIndexBuffer = nullptr;
	}
	return S_OK;
}
HRESULT InputAssembler::SetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppVertexBuffers, const uint32_t* pStrides, const uint32_t* pOffsets)
{
	if (StartSlot + NumBuffers > 4 || !ppVertexBuffers) return E_INVALIDARG;
	std::copy_n(pStrides + StartSlot, NumBuffers, IABufferStrides.begin());
	std::copy_n(pOffsets + StartSlot, NumBuffers, IABufferOffsets.begin());

	for (uint32_t i = StartSlot; i < StartSlot + NumBuffers; i++)
	{
		IAVertexBuffers[i] = dynamic_cast<VBuffer*>(ppVertexBuffers[i]);
	}
	return S_OK;
}
HRESULT InputAssembler::SetInputLayout(IVInputLayout* pInputLayout)
{
	IAInputLayout = dynamic_cast<VInputLayout*>(pInputLayout);
	return S_OK;
}
