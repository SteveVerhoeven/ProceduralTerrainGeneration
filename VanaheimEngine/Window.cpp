#include "pch.h"
#include "Window.h"

// Vanaheim includes
#include "resource.h"
#include "Graphics.h"

// ImGui
#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

Window::Window(const std::string& name, const int width, const int height, HINSTANCE instance)
	   : m_pName(name.c_str())
	   , m_Height(height)
	   , m_Width(width)
	   , m_Instance(instance)
	   , m_Window()
{
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
	// CreateWindowEx - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
	DWORD     dwExStyle = 0;
	LPCSTR    lpClassName = m_pName;
	LPCSTR    lpWindowName = m_pName;
	DWORD     dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	int       X = 100;
	int       Y = 50;
	int       nWidth = m_Width;
	int       nHeight = m_Height;
	HWND      hWndParent = nullptr;
	HMENU     hMenu = nullptr;
	HINSTANCE hInst = m_Instance;
	LPVOID    lpParam = this;
	m_Window = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInst, lpParam);

	// ShowWindow - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	ShowWindow(m_Window, SW_SHOWNORMAL);
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
	/*if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;*/

	//// Messages - Reference: https://wiki.winehq.org/List_Of_Windows_Messages
	//switch (msg)
	//{
	//	case WM_CLOSE:
	//		PostQuitMessage(0);
	//		break;		
	//	default:
	//		break;
	//}

	//return DefWindowProc(hWnd, msg, wParam, lParam);

	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	switch (msg)
	{
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}