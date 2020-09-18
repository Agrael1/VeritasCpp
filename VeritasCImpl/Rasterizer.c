#include "Rasterizer.h"
#include <stdatomic.h>

typedef struct 
{
	int pass;
	float zv;
}DepthTest_t;

HRESULT RasterSetViewPort(Rasterizer* This, uint32_t numVPs, const VVIEWPORT_DESC* in)
{
	This->RSViewPort = *in;
	const float HalfViewportWidth = in->Width * 0.5f;
	const float HalfViewportHeight = in->Height * 0.5f;
	VMStoreFloat4(&This->RSVPScale, VMVectorSet(HalfViewportWidth, -HalfViewportHeight, 1.0f, 0.0f));
	VMStoreFloat4(&This->RSVPOffset, VMVectorSet(HalfViewportWidth, HalfViewportHeight, 1.0f, 0.0f));
	return S_OK;
}

HRESULT __stdcall RasterSetShader(Rasterizer* This, IVShader* pPixelShader)
{
	ComPtrRelease(This->PSPixelShader);

	This->PSPixelShader = pPixelShader;
	if (This->PSPixelShader)
	{
		This->PSPixelShader->method->_unknwn.AddRef((IUnknown*)This->PSPixelShader);
	}
	return S_OK;
}

HRESULT __stdcall RasterSetConstantBuffers(Rasterizer* This, uint32_t StartSlot, uint32_t NumBuffers, IVBuffer* const* ppConstantBuffers)
{
	if (StartSlot + NumBuffers > MaxBuffers) return E_BOUNDS;
	VBuffer** Start = This->PSConstantBuffers + StartSlot;
	VBuffer** End = Start + NumBuffers;

	while (Start != End)
	{
		IUnknown** Buffer = (IUnknown**)Start;
		ComPtrRelease(*Buffer);

		*Start++ = (VBuffer*)*ppConstantBuffers++;
		if ((*Buffer))
		{
			(*Buffer)->method->AddRef(*Buffer);
		}
	}
	return S_OK;
}

void Clip1V(VMVertex* v0, VMVertex* v1, VMVertex* v2, VMVertex _out[3], ShaderPrivateData pd)
{
	uint32_t idx = pd.PositionIndex;

	const float alphaA = (-v0->data[idx].f[2]) / (v1->data[idx].f[2] - v0->data[idx].f[2]);
	const float alphaB = (-v0->data[idx].f[2]) / (v2->data[idx].f[2] - v0->data[idx].f[2]);
	// interpolate to get second triangle
	_out[1] = *v0 = VSOutInterpolate(v0, v1, alphaA, pd.VertexSize);
	_out[0] = VSOutInterpolate(v0, v2, alphaB, pd.VertexSize);
	_out[2] = *v2;
};
void Clip2V(VMVertex* v0, VMVertex* v1, VMVertex* v2, ShaderPrivateData pd)
{
	uint32_t idx = pd.PositionIndex;
	// calculate alpha values for getting adjusted vertices
	const float alpha0 = (-v0->data[idx].f[2]) / (v2->data[idx].f[2] - v0->data[idx].f[2]);
	const float alpha1 = (-v1->data[idx].f[2]) / (v2->data[idx].f[2] - v1->data[idx].f[2]);
	// interpolate to get v0a and v0b

	*v0 = VSOutInterpolate(v0, v2, alpha0, pd.VertexSize);
	*v1 = VSOutInterpolate(v1, v2, alpha1, pd.VertexSize);
};
int RasterClipCullTriangle(VMVertex* TriangleStart, VMVertex _out_opt_AddTri[3], ShaderPrivateData pd)
{
	const XMVECTORU32 preor = {0x80000000, 0x80000000, 0x00000000, 0x00000000};
	const XMVECTORU32 preand = {0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF};

	uint32_t idx = pd.PositionIndex;

	VMVECTOR V0 = TriangleStart[0].data[idx].v;
	VMVECTOR V1 = TriangleStart[1].data[idx].v;
	VMVECTOR V2 = TriangleStart[2].data[idx].v;

	// Compare againgst W value
	VMVECTOR CT0 = VMVectorSplatW(V0);
	VMVECTOR CT1 = VMVectorSplatW(V1);
	VMVECTOR CT2 = VMVectorSplatW(V2);

	VMVECTOR R01 = _mm_cmplt_ps(CT0, V0);
	VMVECTOR R11 = _mm_cmplt_ps(CT1, V1);
	VMVECTOR R21 = _mm_cmplt_ps(CT2, V2);

	VMVECTOR RR1 = _mm_and_ps(_mm_and_ps(R01, R11), R21);
	if (_mm_movemask_ps(RR1) != 0) return false;

	CT0 = _mm_and_ps(_mm_or_ps(CT0, preor.v), preand.v);
	CT1 = _mm_and_ps(_mm_or_ps(CT1, preor.v), preand.v);
	CT2 = _mm_and_ps(_mm_or_ps(CT2, preor.v), preand.v);

	VMVECTOR R02 = _mm_cmplt_ps(V0, CT0);
	VMVECTOR R12 = _mm_cmplt_ps(V1, CT1);
	VMVECTOR R22 = _mm_cmplt_ps(V2, CT2);
	VMVECTOR RR2 = _mm_and_ps(_mm_and_ps(R02, R12), R22);
	if (_mm_movemask_ps(RR2) != 0) return false;

	RR1 = _mm_unpackhi_ps(R02, R12);
	RR2 = _mm_shuffle_ps(RR1, R22, _MM_SHUFFLE(3, 2, 1, 0));

	int selector = _mm_movemask_ps(RR2) & 7;
	switch (selector)
	{
	case 0: return 1;
	case 1: Clip1V(TriangleStart + 0, TriangleStart + 1, TriangleStart + 2, _out_opt_AddTri, pd); return 2;
	case 2: Clip1V(TriangleStart + 1, TriangleStart + 2, TriangleStart + 0, _out_opt_AddTri, pd); return 2;
	case 3: Clip2V(TriangleStart + 0, TriangleStart + 1, TriangleStart + 2, pd); return 1;
	case 4: Clip1V(TriangleStart + 2, TriangleStart + 0, TriangleStart + 1, _out_opt_AddTri, pd); return 2;
	case 5: Clip2V(TriangleStart + 2, TriangleStart + 0, TriangleStart + 1, pd); return 1;
	case 6: Clip2V(TriangleStart + 1, TriangleStart + 2, TriangleStart + 0, pd); return 1;
	}
}

DepthTest_t DepthTest(Rasterizer* This, uint32_t width_in, size_t PremulIndex, float z)
{
	DepthTest_t dt;
	//Warning! lockless programming!
	_Atomic float* reg = (_Atomic float*)(&((float*)This->OMRenderDepth.Scan0)[PremulIndex + width_in]);
	float zv = atomic_load(reg);

	do
	{
		if (z >= zv) return (DepthTest_t) { false, z };
	} while (!atomic_compare_exchange_weak(reg, &zv, z));

	return (DepthTest_t){ true, z };
}

void DrawFlatTriangle
(
	Rasterizer* This,
	const VMVertex* it0,
	const VMVertex* it2,
	const VMVertex* dv0,
	const VMVertex* dv1,
	VMVertex itEdge1,
	ShaderPrivateData pd
)
{
	// create edge interpolant for left edge (always v0)
	VMVertex itEdge0 = *it0;
	VMVertex iLine;
	VMVertex diLine;
	VMVertex _P;
	uint32_t idx = pd.PositionIndex;
	uint32_t attrsize = pd.VertexSize;

	// calculate start and end scanlines (AABB)
	const int yStart = max((int)ceil(it0->data[idx].f[1] - 0.5f), 0);
	const int yEnd = min((int)ceil(it2->data[idx].f[1] - 0.5f), (int)This->RSViewPort.Height); // the scanline AFTER the last line drawn

																						   // do interpolant prestep
	FVMVECTOR step = VMVectorReplicate(((float)yStart + 0.5f - it0->data[idx].f[1]));

	for (unsigned i = 0; i < attrsize; i++)
	{
		itEdge0.data[i].v = VMVectorMultiplyAdd(dv0->data[i].v, step, itEdge0.data[i].v);
		itEdge1.data[i].v = VMVectorMultiplyAdd(dv1->data[i].v, step, itEdge1.data[i].v);
	}

	for (int y = yStart; y < yEnd; y++, VSOutAdd_I(&itEdge0, *dv0, attrsize), VSOutAdd_I(&itEdge1, *dv1, attrsize))
	{
		// calculate start and end pixels
		const int xStart = max((int)ceil(itEdge0.data[idx].f[0] - 0.5f), 0);
		const int xEnd = min((int)ceil(itEdge1.data[idx].f[0]), (int)This->RSViewPort.Width); // the pixel AFTER the last pixel drawn

																								  // create scanline interpolant startpoint
																								  // (some waste for interpolating x,y,z, but makes life easier not having
																								  //  to split them off, and z will be needed in the future anyways...)

		iLine = itEdge0;
		FVMVECTOR step2 = VMVectorReplicate((float)xStart + 0.5f - itEdge0.data[idx].f[0]);
		FVMVECTOR Delta_X = VMVectorReciprocal(VMVectorSplatX(VMVectorSubtract(itEdge1.data[idx].v, itEdge0.data[idx].v)));

		diLine = VSOutMultiply(VSOutSubtract(&itEdge1, &iLine, attrsize), Delta_X, attrsize);
		VSOutAdd_I(&iLine, VSOutMultiply(diLine, step2, attrsize), attrsize);

		const size_t premulI = y * (size_t)(This->RSViewPort.Width);


		for (int x = xStart; x < xEnd; x++, VSOutAdd_I(&iLine, diLine, attrsize))
		{
			DepthTest_t dt = DepthTest(This, x, premulI, iLine.data[idx].f[2]);
			if (dt.pass)
			{
				if (This->PSPixelShader)
				{
					// recover interpolated z from interpolated 1/z
					FVMVECTOR w = VMVectorReciprocalEst(VMVectorSplatW(iLine.data[idx].v));
					for (unsigned i = 0; i < attrsize; i++)
						_P.data[i].v = VMVectorMultiply(iLine.data[i].v, w);
					// invoke pixel shader with interpolated vertex data
					// and use result to set the pixel color on the screen


					uint32_t col;
					This->PSPixelShader->method->Invoke(This->PSPixelShader,&_P, &col);

					if (dt.zv == ((float*)This->OMRenderDepth.Scan0)[premulI + x]) //afxmt sanity check for data races
					{
						((uint32_t*)This->OMRenderTargets[0].Scan0)[premulI + x] = col;
					}
				}
			}
		}
	}
}
void DrawFlatTopTriangle(Rasterizer* This, const VMVertex* Vo0, const VMVertex* Vo1, const VMVertex* Vo2, ShaderPrivateData pd)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	VMVertex dv0 = VSOutSubtract(Vo2, Vo0, pd.VertexSize);
	VMVertex dv1 = VSOutSubtract(Vo2, Vo1, pd.VertexSize);
	float delta_Y = VMVectorGetY(VMVectorReciprocal(dv0.data[pd.PositionIndex].v));

	// delta over 0 and 1 resp
	VSOutScale_I(&dv0, delta_Y, pd.VertexSize);
	VSOutScale_I(&dv1, delta_Y, pd.VertexSize);

	// call the flat triangle render routine, right edge interpolant is it1
	DrawFlatTriangle(This, Vo0, Vo2, &dv0, &dv1, *Vo1, pd);
}
void DrawFlatBottomTriangle(Rasterizer* This, const VMVertex* Vo0, const VMVertex* Vo1, const VMVertex* Vo2, ShaderPrivateData pd)
{
	// calulcate dVertex / dy
	// change in interpolant for every 1 change in y
	VMVertex dv0 = VSOutSubtract(Vo1, Vo0, pd.VertexSize);
	VMVertex dv1 = VSOutSubtract(Vo2, Vo0, pd.VertexSize);
	float delta_Y = VMVectorGetY(VMVectorReciprocal(dv0.data[pd.PositionIndex].v)); // minimize div (reciprocalEst is not good enough)

																			// delta over 1 and 2 resp
	VSOutScale_I(&dv0, delta_Y, pd.VertexSize);
	VSOutScale_I(&dv1, delta_Y, pd.VertexSize);

	// call the flat triangle render routine, right edge interpolant is it0
	DrawFlatTriangle(This, Vo0, Vo2, &dv0, &dv1, *Vo0, pd);
}
void DrawTriangle(Rasterizer* This, VMVertex* TriangleStart, ShaderPrivateData pd)
{
	uint32_t idx = pd.PositionIndex;
	// using pointers so we can swap (for sorting purposes)
	const VMVertex* pv0 = TriangleStart+0;
	const VMVertex* pv1 = TriangleStart+1;
	const VMVertex* pv2 = TriangleStart+2;

	// sorting vertices by y
	if (pv1->data[idx].f[1] < pv0->data[idx].f[1]) swapptr(&pv0, &pv1);
	if (pv2->data[idx].f[1] < pv1->data[idx].f[1]) swapptr(&pv1, &pv2);
	if (pv1->data[idx].f[1] < pv0->data[idx].f[1]) swapptr(&pv0, &pv1);

	if (pv0->data[idx].f[1] == pv1->data[idx].f[1]) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->data[idx].f[0] < pv0->data[idx].f[0]) swapptr(&pv0, &pv1);

		DrawFlatTopTriangle(This, pv0, pv1, pv2, pd);
	}
	else if (pv1->data[idx].f[1] == pv2->data[idx].f[1]) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->data[idx].f[0] < pv1->data[idx].f[0]) swapptr(&pv1, &pv2);

		DrawFlatBottomTriangle(This, pv0, pv1, pv2, pd);
	}
	else // general triangle
	{
		// find splitting vertex interpolant
		const float alphaSplit =
			(pv1->data[idx].f[1] - pv0->data[idx].f[1]) /
			(pv2->data[idx].f[1] - pv0->data[idx].f[1]);
		const VMVertex vi = VSOutInterpolate(pv0, pv2, alphaSplit, pd.VertexSize);

		if (pv1->data[idx].f[0] < vi.data[idx].f[0]) // major right
		{
			DrawFlatBottomTriangle(This, pv0, pv1, &vi, pd);
			DrawFlatTopTriangle(This, pv1, &vi, pv2, pd);
		}
		else // major left
		{
			DrawFlatBottomTriangle(This, pv0, &vi, pv1, pd);
			DrawFlatTopTriangle(This, &vi, pv1, pv2, pd);
		}
	}
}
bool RasterPostProcessTriangle(const Rasterizer* This, VMVertex* TriangleStart, ShaderPrivateData pd)
{
	uint32_t idx = pd.PositionIndex;
	VMVertex* v0 = TriangleStart;
	VMVertex* v1 = TriangleStart + 1;
	VMVertex* v2 = TriangleStart + 2;

	// homo -> NDC space transformation
	VMVECTOR wInv = VMVectorReciprocal(_mm_shuffle_ps(VMVectorMergeZW(v0->data[idx].v, v1->data[idx].v), v2->data[idx].v, _MM_SHUFFLE(3, 3, 3, 2)));
	VMMATRIX X;
	X.r[0] = VMVectorSplatX(wInv); // 1/w0
	X.r[1] = VMVectorSplatY(wInv); // 1/w1
	X.r[2] = VMVectorSplatZ(wInv); // 1/w2
	
	//Screen space transform, 1/w is stored in W
	v0->data[idx].v = VMVectorMultiplyAdd(VMVectorMultiply(v0->data[idx].v, X.r[0]), VMLoadFloat4(&This->RSVPScale), VMLoadFloat4(&This->RSVPOffset)); VMStoreFloat(&v0->data[idx].f[3], X.r[0]);
	v1->data[idx].v = VMVectorMultiplyAdd(VMVectorMultiply(v1->data[idx].v, X.r[1]), VMLoadFloat4(&This->RSVPScale), VMLoadFloat4(&This->RSVPOffset)); VMStoreFloat(&v1->data[idx].f[3], X.r[1]);
	v2->data[idx].v = VMVectorMultiplyAdd(VMVectorMultiply(v2->data[idx].v, X.r[2]), VMLoadFloat4(&This->RSVPScale), VMLoadFloat4(&This->RSVPOffset)); VMStoreFloat(&v2->data[idx].f[3], X.r[2]);
	
	
	XMVECTORF32 x = { .v = VMVector3Cross(VMVectorSubtract(v1->data[idx].v, v0->data[idx].v), VMVectorSubtract(v2->data[idx].v, v0->data[idx].v)) };
	//cull backfaces
	if (x.f[2] < 0.0f)
		return false;
	
	for (unsigned i = 0; i < pd.VertexSize; i++)
	{
		if (i == idx) continue;
		v0->data[i].v = VMVectorMultiply(v0->data[i].v, X.r[0]);
		v1->data[i].v = VMVectorMultiply(v1->data[i].v, X.r[1]);
		v2->data[i].v = VMVectorMultiply(v2->data[i].v, X.r[2]);
	}
	return true;
}

size_t RasterGetSize(const Rasterizer* This)
{
	const VVIEWPORT_DESC* x = &This->RSViewPort;
	return (x->Height - x->TopLeftY) * (x->Width - x->TopLeftX);
}

void RasterizeTriangle(Rasterizer* This, VMVertex* TriangleStart, ShaderPrivateData pd)
{
	VMVertex AddTri[3];
	int out = RasterClipCullTriangle(TriangleStart, AddTri, pd);
	if (!out) return;
	if (out == 2)
	{
		if (RasterPostProcessTriangle(This, AddTri, pd))
		{
			DrawTriangle(This, AddTri, pd);
		}
	}
	if (RasterPostProcessTriangle(This, TriangleStart, pd))
	{
		DrawTriangle(This, TriangleStart, pd);
	}
}
