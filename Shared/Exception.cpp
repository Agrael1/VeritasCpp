#include "Exception.h"
#include <fmt\printf.h>
#include <wil\win32_helpers.h>

Exception::Exception(unsigned int line, const wchar_t * file) noexcept
	:line(line), file(file)
{}

const wchar_t * Exception::GetType() const noexcept
{
	return L"Veritas Exception";
}
unsigned int Exception::GetLine() const noexcept
{
	return line;
}
std::wstring_view Exception::GetFile() const noexcept
{
	return file;
}
std::wstring Exception::GetOriginString() const noexcept
{
	return fmt::sprintf(L"[File]: %s\n[Line]: %u", file, line);
}
const wchar_t* Exception::what() const noexcept
{
	if (whatBuffer.empty())
	{
		whatBuffer = fmt::sprintf(L"%s\n%s", GetType(), GetOriginString());
	}
	return whatBuffer.c_str();
}

// HRESULT Exception
HrException::HrException(int line, const wchar_t* file, HRESULT hr)
	:Exception(line, file), hResult(hr)
{}

std::wstring HrException::TranslateErrorCode(HRESULT hr) noexcept
{
	wil::unique_hlocal_string msg;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&msg,
		0, nullptr);

	if (nMsgLen == 0) return L"Unknown error";
	return msg.get();
}
HRESULT HrException::GetErrorCode() const noexcept
{
	return hResult;
}
const wchar_t* HrException::GetType()const noexcept
{
	return L"Vertas Window Exception";
}
const wchar_t* HrException::what() const noexcept
{
	if (whatBuffer.empty())
	{
		whatBuffer = fmt::sprintf(L"%s\n"
			L"[Error Code]: 0x%X (%lu)\n"
			L"[Description]: %s\n"
			L"%s", GetType(), hResult, (unsigned long)hResult, TranslateErrorCode(hResult), GetOriginString());
	}
	return whatBuffer.c_str();
}
