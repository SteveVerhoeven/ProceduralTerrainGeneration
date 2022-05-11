#include "VanaheimPCH.h"
#include "Window.h"

// Vanaheim includes
#include "resource.h"
#include "Graphics.h"

// ImGui
#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

Window::Window(const std::string& name, HINSTANCE instance)
	   : m_pName(name.c_str())
	   , m_Instance(instance)
	   , m_Window()
{
	GetWidthHeight();
	CreateClass();
	CreateShowWindow();
}
Window::~Window()
{
	DestroyWindow(m_Window);
}

void Window::CreateClass()
{
	// WNDCLASSEX - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProcedureStatic;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_Instance;
	wc.hIcon = static_cast<HICON>(LoadImage(m_Instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_pName;
	wc.hIconSm = static_cast<HICON>(LoadImage(m_Instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

	// RegisterClassEx - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
	RegisterClassEx(&wc);
}
void Window::CreateShowWindow()
{
	// Change the size of the actual user window
	RECT windowRect{};
	windowRect.left = 100;
	windowRect.right = m_Width + windowRect.left;
	windowRect.top = 100;
	windowRect.bottom = m_Height + windowRect.top;
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// CreateWindowA - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowa
	LPCSTR    lpClassName = m_pName;
	LPCSTR    lpWindowName = m_pName;
	/** Window Styles - Reference: https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
		WS_CAPTION: The window has a title bar (includes the WS_BORDER style).
		WS_MINIMIZEBOX: The window has a minimize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified.
		WS_SYSMENU: The window has a window menu on its title bar. The WS_CAPTION style must also be specified.
	*/
	DWORD     dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	int       X = CW_USEDEFAULT;
	int       Y = CW_USEDEFAULT;
	int       nWidth = windowRect.right - windowRect.left;
	int       nHeight = windowRect.bottom - windowRect.top;
	HWND      hWndParent = nullptr;
	HMENU     hMenu = nullptr;
	HINSTANCE hInst = m_Instance;
	LPVOID    lpParam = this;
	m_Window = CreateWindow(lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInst, lpParam);

	// ShowWindow - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	ShowWindow(m_Window, SW_SHOWDEFAULT);
}

LRESULT Window::WindowProcedureStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// WM_CREATE - Reference: https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-create
	if (msg == WM_CREATE)
	{
		CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* pWindow{ reinterpret_cast<Window*>(pCS->lpCreateParams) };
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG>(pCS->lpCreateParams));
		if (pWindow)
			return pWindow->WindowProcedure(hWnd, msg, wParam, lParam);
	}
	else
	{
		Window* pWindow{ reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA)) };
		if (pWindow) 
			return pWindow->WindowProcedure(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Window::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(69);
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::GetWidthHeight()
{
	HWND desktopWindowHandle = GetDesktopWindow();
	HMONITOR monitor = MonitorFromWindow(desktopWindowHandle, MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	m_Width = info.rcMonitor.right - info.rcMonitor.left;
	m_Height = info.rcMonitor.bottom - info.rcMonitor.top;
}