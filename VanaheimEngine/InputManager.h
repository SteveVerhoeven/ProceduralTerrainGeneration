#pragma once
#include <XInput.h>
#include "InputData.h"
#include <unordered_map>

class Command;
class InputManager final
{
	public:
		InputManager();
		~InputManager();

		MSG ProcessInput(const float elapsedSec);

		void AddKeyToMap(const ControllerButton& cButton, const KeyboardButton& kButton, const ButtonPressType& pressType, const std::string& name, Command* const pCommand);

		POINT GetMousePosition();
		POINT GetMouseMovement() const;

	private:
		bool m_MouseInputAccepted = false;

		// Mouse position
		POINT m_MousePosition;
		POINT m_OldMousePosition;

		// Information about the controller/keyboard
		XINPUT_STATE m_ControllerState;

		// General
		std::vector<InputData*> m_pInputs;

		bool ProcessWindowsEvents(MSG& msg);
		bool ProcessGameInput(MSG& msg);

		// Retrieve the command from a Controller button
		Command* GetCommand(const ControllerButton& cButton);
		// Retrieve the command from a Keyboard button
		Command* GetCommand(const KeyboardButton& cButton);
};