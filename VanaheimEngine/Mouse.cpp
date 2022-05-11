#include "VanaheimPCH.h"
#include "Mouse.h"

#include "Window.h"

Mouse::Mouse()
	  : m_MouseInputAccepted(false)
	  , m_MousePosition()
	  , m_OldMousePosition()
	  , m_MouseMovement()
{
	Window* pWindow{ Locator::GetWindowService() };
	m_MousePosition.x = pWindow->GetWindowWidth() / 2;
	m_MousePosition.y = pWindow->GetWindowHeight() / 2;
}

void Mouse::ProcessMouseInputs()
{
	UpdateMousePosition();
	UpdateMouseMovement();
}

void Mouse::UpdateMousePosition()
{
	m_OldMousePosition.x = m_MousePosition.x;
	m_OldMousePosition.y = m_MousePosition.y;

	// ***************************
	// Getting the mouse position
	// ***************************
	// Reference - HOW TO: https://stackoverflow.com/questions/20175342/windows-to-directx-mouse-coordinates
	Window* pWindow{ Locator::GetWindowService() };
	/* GetCursorPos - Parameters */
	LPPOINT lpPoint{ &m_MousePosition };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getcursorpos
	if (GetCursorPos(lpPoint))
	{
		// *****************************************************
		// Convert Screen coordinates to client-area coordinates
		// *****************************************************
		/* ScreenToClient - Parameters */
		HWND    hWnd{ pWindow->GetWindowHandle() };
		// LPPOINT lpPoint{}; 

		// Explanation for all parameters in link below
		// Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-screentoclient
		ScreenToClient(hWnd, lpPoint);
	}
}

void Mouse::UpdateMouseMovement()
{
	m_MouseMovement.x = m_MousePosition.x - m_OldMousePosition.x;
	m_MouseMovement.y = m_MousePosition.y - m_OldMousePosition.y;
}