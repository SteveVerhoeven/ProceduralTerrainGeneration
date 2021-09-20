#pragma once
#include "Command.h"

class RotateCameraCommand final : public Command
{
	public:
		RotateCameraCommand();

		void Execute() override;
		void SetMousePos(const DirectX::XMFLOAT2& mousePos) { m_MousePos = mousePos; }

	private:
		float m_RotatingSpeed;
		DirectX::XMFLOAT2 m_MousePos;
		GameObject* m_pCameraObject;

		void Rotate();
		void SetCameraObject();
};