#include "pch.h"
#include "MoveCameraCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

MoveCameraCommand::MoveCameraCommand(const bool moveForward, const bool moveBackward, const bool moveLeft, const bool moveRight)
				  : m_MoveForward(moveForward)
				  , m_MoveBackward(moveBackward)
				  , m_MoveLeft(moveLeft)
				  , m_MoveRight(moveRight)
				  , m_MoveSpeed(500.f)
				  , m_pCameraObject(nullptr)
{
	SetCameraObject();
}

void MoveCameraCommand::Execute()
{
	Move();
}

void MoveCameraCommand::Move()
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	TransformComponent* pTransformComponent{ m_pCameraObject->GetComponent<TransformComponent>() };
	const DirectX::XMFLOAT3 currentPosition{ pTransformComponent->GetWorldPosition() };
	const DirectX::XMVECTOR currentPositionVector{ DirectX::XMLoadFloat3(&currentPosition) };

	const float elapsedSec{ Locator::GetTimerService()->GetElapsedTime() };

	// ****************************
	// Move LEFT / RIGHT
	// ****************************
	if (m_MoveLeft || m_MoveRight)
	{
		const DirectX::XMFLOAT3 right = pTransformComponent->GetRight();
		const DirectX::XMVECTOR rightVector{ DirectX::XMLoadFloat3(&right) };

		DirectX::XMVECTOR newPosition{};
		if (m_MoveLeft)
			newPosition = currentPositionVector - (rightVector * m_MoveSpeed * elapsedSec);
		else if (m_MoveRight)
			newPosition = currentPositionVector + (rightVector * m_MoveSpeed * elapsedSec);

		pTransformComponent->Translate(newPosition, false);
	}

	// ****************************
	// Move FORWARD / BACKWARD
	// ****************************
	if (m_MoveForward || m_MoveBackward)
	{
		const DirectX::XMFLOAT3 forward = pTransformComponent->GetForward();
		const DirectX::XMVECTOR forwardVector{ DirectX::XMLoadFloat3(&forward) };

		DirectX::XMVECTOR newPosition{};
		if (m_MoveForward)
			newPosition = currentPositionVector + (forwardVector * m_MoveSpeed * elapsedSec);
		else if (m_MoveBackward)
			newPosition = currentPositionVector - (forwardVector * m_MoveSpeed * elapsedSec);

		pTransformComponent->Translate(newPosition, false);
	}	
}

void MoveCameraCommand::SetCameraObject()
{
	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	Scene* pScene{ pSceneManager->GetActiveGameScene() };
	m_pCameraObject = pScene->GetObjectByName("Camera-Main");
}