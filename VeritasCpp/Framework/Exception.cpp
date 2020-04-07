#include "Exception.h"
#include <sstream>

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
const std::wstring& Exception::GetFile() const noexcept
{
	return file;
}
std::wstring Exception::GetOriginString() const noexcept
{
	std::wostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
const wchar_t* Exception::what() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
