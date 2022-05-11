#include "VanaheimPCH.h"
#pragma comment(lib, "XInput.lib")
#include "InputManager.h"

#include "Command.h"
#include "RotateCameraCommand.h"
#include "Window.h"
#include "Mouse.h"

InputManager::InputManager()
			 : m_ControllerState()
			 , m_pInputs(std::vector<InputData*>())
			 , m_pMouse(new Mouse())
			 , m_QuitGame(false)
{}
InputManager::~InputManager()
{
	for (InputData* pInputData : m_pInputs)
		DELETE_POINTER(pInputData);

	m_pInputs.clear();
	DELETE_POINTER(m_pMouse);
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

	if (m_QuitGame)
	{
		msg.message = WM_QUIT;
		return msg;
	}

	// Update mouse movement
	m_pMouse->ProcessMouseInputs();

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

		if (msg.wParam == 'G' && msg.message == WM_KEYUP)
		{
			if (m_pMouse->GetMouseActive())
				m_pMouse->DeactivateMouseInput();
			else
				m_pMouse->ActivateMouseInput();
		}

		// Mouse rotation
		/* TODO: Snapping problem: When moving the mouse, it starts from the old position instead of the new one */
		if (m_pMouse->GetMouseActive() && msg.message == WM_MOUSEMOVE)
		{
			LOG(ErrorLevel::LOG_INFO, "Moving\n");

			POINT mouseMov{ m_pMouse->GetMouseMovement() };

			DirectX::XMFLOAT2 mouse{};
			mouse.x = float(mouseMov.x);
			mouse.y = float(mouseMov.y);

			Command* pCommand{ GetCommand(ControllerButton::ButtonLThumbStick) };
			RotateCameraCommand* pRotateCommand{ dynamic_cast<RotateCameraCommand*>(pCommand) };
			pRotateCommand->SetMouseMovement(mouse);
			pCommand->Execute();
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