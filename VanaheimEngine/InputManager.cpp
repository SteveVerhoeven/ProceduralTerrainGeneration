#include "pch.h"
#pragma comment(lib, "XInput.lib")
#include "InputManager.h"

#include "Command.h"
#include "RotateCameraCommand.h"

#include "Window.h"
#include "imgui.h"

InputManager::InputManager()
			 : m_MousePosition()
			 , m_OldMousePosition()
			 , m_ControllerState()
			 , m_pInputs(std::vector<InputData*>())
{
	Window* pWindow{ Locator::GetWindowService() };
	m_MousePosition.x = pWindow->GetWindowWidth() / 2;
	m_MousePosition.y = pWindow->GetWindowHeight() / 2;
}
InputManager::~InputManager()
{
	for (InputData* pInputData : m_pInputs)
		DELETE_POINTER(pInputData);

	m_pInputs.clear();
}

MSG InputManager::ProcessInput(const float /*elapsedSec*/)
{
	MSG msg{};
	ZeroMemory(&msg, sizeof(msg));

	if (!ProcessWindowsEvents(msg))
		return msg;

	if (!ProcessGameInput(msg))
	{
		msg.message = WM_QUIT;
		return msg;
	}

	return msg;
}
bool InputManager::ProcessWindowsEvents(MSG& msg)
{
	// PeekMessage - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return false;

		if (msg.wParam == 'G')
		{
			m_MouseInputAccepted = !m_MouseInputAccepted;
		}

		// Mouse rotation
		if (m_MouseInputAccepted)
		{
			if (msg.wParam == VK_LBUTTON)
			{
				if (msg.message == WM_MOUSEMOVE)
				{
					POINT mousePos{ GetMousePosition() };
					POINT mouseMov{ GetMouseMovement() };

					DirectX::XMFLOAT2 mouse{};
					mouse.x = float(mouseMov.x);
					mouse.y = float(mouseMov.y);

					Command* pCommand{ GetCommand(ControllerButton::ButtonLThumbStick) };
					RotateCameraCommand* pRotateCommand{ dynamic_cast<RotateCameraCommand*>(pCommand) };
					pRotateCommand->SetMousePos(mouse);
					pCommand->Execute();
				}
			}
		}
	}

	return true;
}
bool InputManager::ProcessGameInput(MSG& msg)
{
	// PeekMessage - Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		for (const InputData* pInputData : m_pInputs)
		{
			bool executeCommand{ false };
			// ******************************************************************
			// Asking the state of the registered input key for the current scene
			// ******************************************************************
			/* GetKeyState - parameters */
			int nVirtKey{ int(pInputData->keyData.keyboardButton) };
			// Reference: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getkeystate
			if (GetKeyState(nVirtKey) & 0x8000)
			{
				executeCommand = true;
			}

			if (executeCommand)
			{
				Command* pCommand{ GetCommand(pInputData->keyData.keyboardButton) };
				pCommand->Execute();
				 
				return true;
			}
		}
	}

	return true;
}

void InputManager::AddKeyToMap(const ControllerButton& cButton, const KeyboardButton& kButton, const ButtonPressType& pressType, const std::string& name, Command* const pCommand)
{
	InputData* pInputData{ new InputData() };
	KeyData keyData{};
	keyData.buttonState = pressType;
	keyData.controllerButton = cButton;
	keyData.keyboardButton = kButton;
	CommandData commandData{};
	commandData.commandName = name;
	commandData.pCommand = pCommand;

	pInputData->commandData = commandData;
	pInputData->keyData = keyData;

	m_pInputs.push_back(pInputData);
}


// Retrieve the command from a Controller button
Command* InputManager::GetCommand(const ControllerButton& cButton)
{
	auto result = std::find_if(m_pInputs.begin(), m_pInputs.end(), [&](InputData* pInputData) 
	{ return pInputData->keyData.controllerButton == cButton; });

	if (result != m_pInputs.end())
		return (*result)->commandData.pCommand;

	return nullptr;
}
// Retrieve the command from a Keyboard button
Command* InputManager::GetCommand(const KeyboardButton& kButton)
{
	auto result = std::find_if(m_pInputs.begin(), m_pInputs.end(), [&](InputData* pInputData)
	{ return pInputData->keyData.keyboardButton == kButton; });

	if (result != m_pInputs.end())
		return (*result)->commandData.pCommand;

	return nullptr;
}
POINT InputManager::GetMousePosition()
{
	m_OldMousePosition = m_MousePosition;

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

	return m_MousePosition;
}
POINT InputManager::GetMouseMovement() const
{
	POINT movement{};
	movement.x = m_OldMousePosition.x - m_MousePosition.x;
	movement.y = m_OldMousePosition.y - m_MousePosition.y;

	return movement;
}
