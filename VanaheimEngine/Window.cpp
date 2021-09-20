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
	int       X = 200;
	int       Y = 200;
	int       nWidth = m_Width;
	int       nHeight = m_Height;
	HWND      hWndParent = nullptr;
	HMENU     hMenu = nullptr;
	HINSTANCE hInst = m_Instance;
	LPVOID    lpParam = this;
	m_Window = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInst, lpParam);

	// ShowWindow - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
	ShowWindow(m_Window, SW_SHOWMAXIMIZED);
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
LRESULT Window::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	// Messages - Reference: https://wiki.winehq.org/List_Of_Windows_Messages
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		/*case WM_KEYDOWN:
			if (wParam == 'F')
			{
				std::string newTitle{ "Valhalla" };
				SetWindowText(hWnd, newTitle.c_str());
			}
			break;
		case WM_KEYUP:
			if (wParam == 'F')
			{
				std::string newTitle{ "Odin" };
				SetWindowText(hWnd, newTitle.c_str());
			}
			break;
		case WM_CHAR:
		{
			static std::string title{};
			title.push_back((char)wParam);
			SetWindowText(hWnd, title.c_str());
		}
			break;
		case WM_LBUTTONDOWN:
		{
			POINTS pt{ MAKEPOINTS(lParam) };
			std::ostringstream oss{};
			oss << "(" << pt.x << "," << pt.y << ")";
			SetWindowText(hWnd, oss.str().c_str());
		}
			break;*/
		
		
		default:
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}