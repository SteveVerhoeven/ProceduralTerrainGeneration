#pragma once
class Mouse final
{	
	public:
		Mouse();
		~Mouse() = default;

		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		Mouse(Mouse&&) = delete;
		Mouse& operator=(Mouse&&) = delete;
		
		void ProcessMouseInputs();

		const POINT& GetMousePosition() const { return m_MousePosition; }
		const POINT& GetMouseMovement() const { return m_MouseMovement; }

		void ActivateMouseInput() { m_MouseInputAccepted = true; }
		void DeactivateMouseInput() { m_MouseInputAccepted = false; }
		bool GetMouseActive() const { return m_MouseInputAccepted; }

	private:
		bool m_MouseInputAccepted;

		// position
		POINT m_MousePosition;
		POINT m_OldMousePosition;

		// movement
		POINT m_MouseMovement;

		void UpdateMousePosition();
		void UpdateMouseMovement();
};

