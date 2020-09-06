#include <fmt\printf.h>
#include "GdiException.h"


GdiException::GdiException(Gdiplus::Status in_status, unsigned line, const wchar_t* file) noexcept
    :Exception(line,file),status(in_status)
{

}

const wchar_t* GdiException::GetStatusDescription(Gdiplus::Status status) noexcept
{
    switch (status)
    {
    case Gdiplus::Ok:
        return L"The method call was successful.";
    case Gdiplus::GenericError:
        return L"There was an error on the method call, which is identified as something other than those defined by the other elements of this enumeration.";
    case Gdiplus::InvalidParameter:
        return L"One of the arguments passed to the method was not valid.";
    case Gdiplus::OutOfMemory:
        return L"The operating system is out of memory and could not allocate memory to process the method call.";
    case Gdiplus::ObjectBusy:
        return L"One of the arguments specified in the API call is already in use in another thread.";
    case Gdiplus::InsufficientBuffer:
        return L"A buffer specified as an argument in the API call is not large enough to hold the data to be received.";
    case Gdiplus::NotImplemented:
        return L"The method is not implemented.";
    case Gdiplus::Win32Error:
        return L"The method generated a Win32 error.";
    case Gdiplus::WrongState:
        return L"The object is in an invalid state to satisfy the API call.";
    case Gdiplus::Aborted:
        return L"The method was aborted";
    case Gdiplus::FileNotFound:
        return L"The specified image file or metafile cannot be found.";
    case Gdiplus::ValueOverflow:
        return L"The method performed an arithmetic operation that produced a numeric overflow.";
    case Gdiplus::AccessDenied:
        return L"A write operation is not allowed on the specified file.";
    case Gdiplus::UnknownImageFormat:
        return L"The specified image file format is not known.";
    case Gdiplus::FontFamilyNotFound:
        return L"The specified font family cannot be found.Either the font family name is incorrect or the font family is not installed.";
    case Gdiplus::FontStyleNotFound:
        return L"The specified style is not available for the specified font family.";
    case Gdiplus::NotTrueTypeFont:
        return L"The font retrieved from an HDC or LOGFONT is not a TrueType font and cannot be used with GDI+.";
    case Gdiplus::UnsupportedGdiplusVersion:
        return L"The version of GDI+ that is installed on the system is incompatible with the version with which the application was compiled.";
    case Gdiplus::GdiplusNotInitialized:
        return L"The GDI+ API is not in an initialized state.To function, all GDI+ objects require that GDI+ be in an initialized state. Initialize GDI + by calling GdiplusStartup.";
    case Gdiplus::PropertyNotFound:
        return L"The specified property does not exist in the image.";
    case Gdiplus::PropertyNotSupported:
        return L"The specified property is not supported by the format of the imageand, therefore, cannot be set.";
    default:
        return L"Unknown Error";
    }
}
const wchar_t* GdiException::GetType() const noexcept
{
    return L"Graphics Device Exception";
}
const wchar_t* GdiException::what() const noexcept
{
    if (whatBuffer.empty())
    {
        whatBuffer = fmt::sprintf(L"%s\n"
            L"[Details]: %s\n"
        , GetType(), GetStatusDescription(status));

        if (status == Gdiplus::Status::Win32Error)
        {
            HRESULT hResult = HRESULT_FROM_WIN32(GetLastError());
            std::wstring message = HrException::TranslateErrorCode(hResult);
            whatBuffer += fmt::sprintf(L"[Error Code]: 0x%X (%lu)\n"
                L"[Description]: %s\n" , hResult, (unsigned long)hResult, message);
        }
        whatBuffer += GetOriginString();
    }
    return whatBuffer.c_str();
}
