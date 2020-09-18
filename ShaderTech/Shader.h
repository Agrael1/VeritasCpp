#pragma once
#include <string>
#include <tuple>
#include <span>
#include "ShaderCommon.h"
#include <VeritasMath.h>


struct DumbVertex
{
	dx::XMVECTOR data[16];
	SV_VertexID SV_VertexID;
};
struct DumbVSOut
{
	dx::XMVECTOR attributes[16];
	uint32_t reserved;			//not in use
};
struct DumbPSOut
{
	uint32_t SV_Target;
};

template<class...>struct types { using type = types; };
template<class out, class...Args>struct xtypes { using type = types<Args...>; using Out_t = out; };
template<class Sig> struct args;

template<class Out, class M, class...Args>
struct args<Out(M::*)(Args...)> :xtypes<Out, Args...> {};

template<class Sig> using args_t = typename args<Sig>::type;
template<class Sig> using out_t = typename args<Sig>::Out_t;

template<class T, class C>
constexpr T determine(const dx::XMVECTOR target, SV_VertexID vid)
{
	if constexpr (std::is_same_v<T, C>)
	{
		return vid;
	}
	else
	{
		return target;
	}
}
template <class... Formats, size_t N, size_t... Is>
constexpr std::tuple<Formats...> as_tuple(std::span<const dx::XMVECTOR, N> arr, SV_VertexID vid,
	std::index_sequence<Is...>)
{
	return std::make_tuple(std::forward<Formats>(determine<Formats, SV_VertexID>(arr[Is], vid))...);
}
template <class... Formats, size_t N>
constexpr std::tuple<Formats...> as_tuple(types<Formats...>, std::span<const dx::XMVECTOR, N> arr, SV_VertexID vid = 0)
{
	return as_tuple<Formats...>(arr, vid, std::make_index_sequence<sizeof...(Formats)>{});
}


struct ShaderBase : public wrl::RuntimeClass<wrl::RuntimeClassFlags<wrl::ClassicCom>, IVShader>
{
};

template<class T>
struct VertexShader : public ShaderBase
{
public:
	VertexShader() = default;
public:
	void __stdcall Invoke(const void* vs_in, void* _out_vertex)override
	{
		using RQVSOutT = out_t<decltype(&T::main)>;

		const auto& in = *static_cast<const DumbVertex*>(vs_in);
		auto& out = *static_cast<DumbVSOut*>(_out_vertex);

		RQVSOutT x = std::apply(&T::main,
			std::tuple_cat(std::make_tuple(reinterpret_cast<T*>(this)), as_tuple(args_t<decltype(&T::main)>{}, std::span(in.data), in.SV_VertexID)));

		std::copy((std::byte*)&x, (std::byte*)&x + sizeof(x), (std::byte*)&out.attributes);
	}
	constexpr void __stdcall GetShaderPrivateData(ShaderPrivateData* _out_pdata)override
	{
		using RQVSOutT = out_t<decltype(&T::main)>;
		_out_pdata->VertexSize = sizeof(RQVSOutT) / 16;

		if constexpr (sizeof(RQVSOutT) == sizeof(float4A))
		{
			_out_pdata->PositionIndex = 0;
		}
		else
		{
			static_assert(alignof(decltype(RQVSOutT::SV_Position)) == alignof(float4A));
			static_assert(sizeof(RQVSOutT::SV_Position) == sizeof(float4A));

			_out_pdata->PositionIndex = offsetof(RQVSOutT, SV_Position) / 16;
		}
	}
};


template<class T>
struct PixelShader : public ShaderBase
{
public:
	PixelShader() = default;
public:
	void __stdcall Invoke(const void* ps_in, void* _out_vertex)override
	{
		using RQPSOutT = out_t<decltype(&T::main)>;
		const DumbVSOut& in = *static_cast<const DumbVSOut*>(ps_in);
		DumbPSOut& out = *static_cast<DumbPSOut*>(_out_vertex);

		static_assert(sizeof(RQPSOutT) == sizeof(float4));

		RQPSOutT x = std::apply(&T::main,
			std::tuple_cat(std::make_tuple(reinterpret_cast<T*>(this)), as_tuple(args_t<decltype(&T::main)>{}, std::span(in.attributes))));

		dx::PackedVector::XMStoreColor((dx::PackedVector::XMCOLOR*)&out.SV_Target, x);
	}
	constexpr void __stdcall GetShaderPrivateData(ShaderPrivateData* _out_pdata)override
	{
		_out_pdata->VertexSize = 1;
	}
};