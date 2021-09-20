#pragma once

#include "Command.h"

enum class ControllerButton
{
	NoAction,

	ButtonA = XINPUT_GAMEPAD_A,
	ButtonB = XINPUT_GAMEPAD_B,
	ButtonX = XINPUT_GAMEPAD_X,
	ButtonY = XINPUT_GAMEPAD_Y,

	ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
	ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
	ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
	ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,

	ButtonLThumb = XINPUT_GAMEPAD_LEFT_THUMB,
	ButtonRThumb = XINPUT_GAMEPAD_RIGHT_THUMB,

	ButtonLShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
	ButtonRShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,

	ButtonLTrigger,
	ButtonRTrigger,

	ButtonTrigger_DEADZONE = XINPUT_GAMEPAD_TRIGGER_THRESHOLD,

	ButtonLThumbStick_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE,
	ButtonRThumbStick_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE,

	ButtonLThumbStick,
	ButtonRThumbStick,

	ButtonBack = XINPUT_GAMEPAD_BACK,
	ButtonStart = XINPUT_GAMEPAD_START
};
enum class ButtonPressType
{
	BUTTON_HOLD,
	BUTTON_PRESSED,
	BUTTON_RELEASED
};
enum class KeyboardButton
{
	// Reference: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	ESC,

	// F-KEYS
	F1 = VK_F1,
	F2 = VK_F2,
	F3 = VK_F3,
	F4 = VK_F4,
	F5 = VK_F5,
	F6 = VK_F6,
	F7 = VK_F7,
	F8 = VK_F8,
	F9 = VK_F9,
	F10 = VK_F10,
	F11 = VK_F11,
	F12 = VK_F12,
	F13 = VK_F13,
	F14 = VK_F14,
	F15 = VK_F15,
	F16 = VK_F16,
	F17 = VK_F17,
	F18 = VK_F18,
	F19 = VK_F19,
	F20 = VK_F20,
	F21 = VK_F21,
	F22 = VK_F22,
	F23 = VK_F23,
	F24 = VK_F24,

	// NUMBERS-KEYS
	ZERO  = 0x30,
	ONE   = 0x31,
	TWO   = 0x32,
	THREE = 0x33,
	FOUR  = 0x34,
	FIVE  = 0x35,
	SIX   = 0x36,
	SEVEN = 0x37,
	EIGHT = 0x38,
	NINE  = 0x39,

	ZERO_NUMPAD  = VK_NUMPAD0,
	ONE_NUMPAD   = VK_NUMPAD1,
	TWO_NUMPAD   = VK_NUMPAD2,
	THREE_NUMPAD = VK_NUMPAD3,
	FOUR_NUMPAD  = VK_NUMPAD4,
	FIVE_NUMPAD  = VK_NUMPAD5,
	SIX_NUMPAD   = VK_NUMPAD6,
	SEVEN_NUMPAD = VK_NUMPAD7,
	EIGHT_NUMPAD = VK_NUMPAD8,
	NINE_NUMPAD  = VK_NUMPAD9,

	// LETTERS-KEYS
	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
	Z = 0x5A,

	// ARROWS-KEYS
	ARROW_LEFT  = VK_LEFT,
	ARROW_RIGHT = VK_RIGHT,
	ARROW_UP    = VK_UP,
	ARROW_DOWN  = VK_DOWN,
};

struct KeyData
{
	KeyData()
		: KeyData(ControllerButton::NoAction, KeyboardButton::A, ButtonPressType::BUTTON_RELEASED)
	{}
	KeyData(const KeyData& kData)
		: controllerButton(kData.controllerButton)
		, keyboardButton(kData.keyboardButton)
		, buttonState(kData.buttonState)
	{}
	KeyData(const ControllerButton& cButton, 
			const KeyboardButton& kButton, 
			const ButtonPressType& pressType)
		: controllerButton(cButton)
		, keyboardButton(kButton)
		, buttonState(pressType)
	{}
	~KeyData()
	{}

	ControllerButton controllerButton{};
	KeyboardButton	 keyboardButton{};
	ButtonPressType  buttonState{};
};
struct CommandData
{
	CommandData()
		: CommandData("", nullptr)
	{}
	CommandData(const CommandData& cData)
		: commandName(cData.commandName)
		, pCommand(cData.pCommand)
	{}
	CommandData(const std::string& cName, Command* pCommand)
		: commandName(cName)
		, pCommand(pCommand)
	{}
	~CommandData()
	{}

	std::string commandName{};
	Command* pCommand{};
};

struct InputData
{
	InputData()
		:InputData({}, {})
	{}
	InputData(const InputData& iData)
		: keyData(iData.keyData)
		, commandData(iData.commandData)
	{}
	InputData(const KeyData& kData, const CommandData& cData)
		: keyData(kData)
		, commandData(cData)
	{}
	~InputData()
	{ DELETE_POINTER(commandData.pCommand); }

	KeyData keyData{};
	CommandData commandData{};
};