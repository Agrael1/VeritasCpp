#include "Window.h"
#include <sstream>

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Create routine initializer
	if (msg == WM_NCCREATE)
	{
		// Extract data from creation of window
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to win class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set msgproc to to non setup handle
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to win class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward msg to class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{	
	case WM_CLOSE:{
		PostQuitMessage(0);// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
		return 0;
	}
	case WM_KILLFOCUS: {	// clear keystate when window loses focus to prevent input getting "stuck"
		kbd.ClearState();
		inFocus = false;
		break;
	}
	case WM_SETFOCUS:
		inFocus = true;
	case WM_ACTIVATE: {
		if (!cursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;
	}

	/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: {// syskey commands need to be handled to track ALT key (VK_MENU) and F10
		// stifle this keyboard message if imgui wants to capture
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		// stifle this keyboard message if imgui wants to capture
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR: {
		// stifle this keyboard message if imgui wants to capture
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
	/*********** END KEYBOARD MESSAGES ***********/

	/************* MOUSE MESSAGES ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		if (!cursorEnabled)
		{
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
				HideCursor();
			}
			break;
		}
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!cursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/************** END MOUSE MESSAGES **************/

	/************** RAW MOUSE MESSAGES **************/
	case WM_INPUT:
	{
		if (!mouse.RawEnabled())
		{
			break;
		}
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			break;
		}
	
		if(rawBuffer.size()!=size) rawBuffer.resize(size);

		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			break;
		}

		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}
	/************** END RAW MOUSE MESSAGES **************/
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Window()
	:hInst(GetModuleHandle(nullptr))
{
	AllocConsole();
	hWindow = GetConsoleWindow();
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = hOriginalConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// create a control subwindow
	WNDCLASSEX wx = { 0 };
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = HandleMsgSetup;        // function which will handle messages
	wx.hInstance = hInst;
	wx.lpszClassName = wndClassName;

	if (RegisterClassEx(&wx))
	{
		ControlWindow = CreateWindow(
			wndClassName,
			L"dummy_name",
			WS_CHILD, 0, 0, 0, 0,
			hWindow,
			NULL, hInst, this);

		SetFocus(ControlWindow);
	}
}
Window::~Window()
{
	UnregisterClass(wndClassName, GetInstance());
	FreeConsole();
}

void Window::SetFont(BYTE fontw, BYTE fonth) const
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, 9, L"Consolas");

	WND_CALL_INFO(SetCurrentConsoleFontEx(hOut, false, &cfi));
}
void Window::CreateConsole(WORD width, WORD height, BYTE fontw, BYTE fonth)
{
	SetWindowLongPtr(hWindow, GWL_STYLE,
		GetWindowLong(hWindow, GWL_STYLE)
		& ~WS_SIZEBOX & ~WS_SYSMENU & ~WS_MINIMIZEBOX);

	cDimentions.Y = height;
	cDimentions.X = width;

	hOut = CreateConsoleScreenBuffer(
		GENERIC_READ |           // read/write access 
		GENERIC_WRITE,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE,        // shared 
		NULL,                    // default security attributes 
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
		NULL);

	WND_CALL_INFO(hOut != INVALID_HANDLE_VALUE);
	WND_CALL_INFO(SetConsoleActiveScreenBuffer(hOut));
	SetFont(fontw, fonth);

	COORD coordLargest = GetLargestConsoleWindowSize(hOut);
	if (height > coordLargest.Y)
		cDimentions.Y = coordLargest.Y;
	if (width > coordLargest.X)
		cDimentions.X = coordLargest.X;

	// Set size of buffer
	WND_CALL_INFO(SetConsoleScreenBufferSize(hOut, cDimentions));

	// Set Physical Console Window Size
	rWindowRect = SMALL_RECT{ 0, 0, cDimentions.X - 1, cDimentions.Y - 1 };
	WND_CALL_INFO(SetConsoleWindowInfo(hOut, TRUE, &rWindowRect));

	RECT windowR = { 0 };
	WND_CALL_INFO(GetClientRect(hWindow, &windowR));
	width = (short)windowR.right;
	height = (short)windowR.bottom;
}

void Window::Restore()
{
	WND_CALL_INFO(SetConsoleActiveScreenBuffer(hOriginalConsole));
}
void Window::SetConsoleCursor(bool value)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = value;
	WND_CALL_INFO(SetConsoleCursorInfo(hOut, &info));
}
void Window::OutputToScreen(const std::vector<CHAR_INFO>& buffer)
{
	WriteConsoleOutputW(hOut, buffer.data(), cDimentions, {	0, 0 },	&rWindowRect);
}
void Window::SetPalette(COLORREF palette[16])
{
	CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hOut, &csbiex);
	csbiex.dwSize.Y--;					// a little fix of scrollbars
	memcpy(csbiex.ColorTable, palette, 16 * sizeof(COLORREF));
	SetConsoleScreenBufferInfoEx(hOut, &csbiex);
}

void Window::ConfineCursor() noexcept
{
	RECT rekt;
	GetWindowRect(hWindow, &rekt);
	rekt.right = rekt.left + 1;
	ClipCursor(&rekt);
}
void Window::FreeCursor() noexcept
{
	ClipCursor(nullptr);
}
void Window::HideCursor() noexcept
{
	while (::ShowCursor(FALSE) >= 0);
}
void Window::ShowCursor() noexcept
{
	while (::ShowCursor(TRUE) < 0);
}
bool Window::InFocus() noexcept
{
	return inFocus;
}
void Window::EnableCursor() noexcept
{
	cursorEnabled = true;
	ShowCursor();
	FreeCursor();
}
void Window::DisableCursor() noexcept
{
	cursorEnabled = false;
	HideCursor();
	ConfineCursor();
}
bool Window::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

void Window::CatchFocus()
{
	DWORD numRecords;
	DWORD numRecsRecieved;
	GetNumberOfConsoleInputEvents(hIn, &numRecords);
	PINPUT_RECORD records = static_cast<INPUT_RECORD*>(_malloca(numRecords * sizeof(INPUT_RECORD)));
	ReadConsoleInput(hIn, records, numRecords, &numRecsRecieved);

	for (DWORD i = 0; i < numRecsRecieved; i++)
	{
		if (records[i].EventType == FOCUS_EVENT && records[i].Event.FocusEvent.bSetFocus)
		{
			SetFocus(ControlWindow);
		}
	}
	_freea(records);
}
std::optional<WPARAM> Window::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return{};
}

// Window Exception
Window::HrException::HrException(int line, const wchar_t* file, HRESULT hr)
	:WindowException(line, file), hResult(hr)
{}
std::wstring Window::WindowException::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pMsgBuf),
		0, nullptr);

	if (nMsgLen == 0)
	{
		return L"Unknown error";
	}
	std::wstring errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}
HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hResult;
}
std::wstring Window::HrException::GetErrorDescription() const noexcept
{
	return WindowException::TranslateErrorCode(hResult);
}
const wchar_t* Window::HrException::GetType()const noexcept
{
	return L"Vertas Window Exception";
}
const wchar_t* Window::HrException::what() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const wchar_t* Window::NoGfxException::GetType() const noexcept
{
	return L"Veritas Window Exception [No Graphics]";
}
