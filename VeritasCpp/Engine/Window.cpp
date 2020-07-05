#include "Window.h"

//Window Class winstyle
Window::WindowClass Window::WindowClass::wndclass;

Window::WindowClass::WindowClass()noexcept
	:hinst( GetModuleHandle(nullptr) )
{
	appico.reset(LoadIcon(hinst, MAKEINTRESOURCE(IDI_ICON1)));
	WNDCLASSEX wcWindow = { 0 };
	wcWindow.cbSize = sizeof(wcWindow);
	wcWindow.style = CS_OWNDC;
	wcWindow.lpfnWndProc = HandleMsgSetup;
	wcWindow.cbClsExtra = 0;
	wcWindow.cbWndExtra = 0;
	wcWindow.hInstance = hinst;
	wcWindow.hCursor = nullptr;
	wcWindow.hIcon = appico.get();
	wcWindow.hbrBackground = nullptr;
	wcWindow.lpszMenuName = nullptr;
	wcWindow.hIconSm = nullptr;
	wcWindow.lpszClassName = GetName();
	RegisterClassEx(&wcWindow);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, hinst);
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndclass.hinst;
}



Window::Window(uint32_t width, uint32_t height, std::wstring_view name)
	:width(width),height(height)
{
	RECT rWindow;
	rWindow.left = 100;
	rWindow.right = width + rWindow.left;
	rWindow.top = 100;
	rWindow.bottom = height + rWindow.top;
	// Automatic calculation of window height and width to client region
	WND_CALL_INFO(AdjustWindowRect(&rWindow, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE));

	hWindow.reset(CreateWindow(
		WindowClass::GetName(), name.data(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rWindow.right - rWindow.left,
		rWindow.bottom - rWindow.top,
		nullptr, nullptr,
		WindowClass::GetInstance(), this
	));

	// Error checks
	if (!hWindow) throw WND_LAST_EXCEPT();
	ShowWindow(hWindow.get(), SW_SHOWDEFAULT);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	WND_CALL_INFO(RegisterRawInputDevices(&rid, 1, sizeof(rid)));
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept
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
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept
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
		break;
	}
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
		if (pt.x >= 0 && pt.x < (short)width && pt.y >= 0 && pt.y < (short)height)
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
		if (pt.x < 0 || pt.x >= (short)width || pt.y < 0 || pt.y >= (short)height)
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
		if (pt.x < 0 || pt.x >= (short)width || pt.y < 0 || pt.y >= (short)height)
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

HWND Window::GetWindowHandle() const noexcept
{
	return hWindow.get();
}

void Window::ConfineCursor() noexcept
{
	RECT rekt;
	GetWindowRect(hWindow.get(), &rekt);
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

