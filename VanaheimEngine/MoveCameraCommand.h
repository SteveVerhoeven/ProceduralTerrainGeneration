#pragma once
#include "Command.h"

class GameObject;
class MoveCameraCommand final : public Command
{
	public:
		MoveCameraCommand(const bool moveForward, 
						  const bool moveBackward, 
						  const bool moveLeft, 
						  const bool moveRight);

		void Execute() override;

	private:
		bool m_MoveForward,
		     m_MoveLeft,
		     m_MoveBackward,
		     m_MoveRight;
		float m_MoveSpeed;
		GameObject* m_pCameraObject;

		void Move();
		void SetCameraObject();
};