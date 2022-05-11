#include "VanaheimPCH.h"
#include "RotateCameraCommand.h"

#include "Scene.h"
#include "Timer.h"

RotateCameraCommand::RotateCameraCommand()
					: m_RotatingSpeed(20.f)
					, m_MouseMovement()
					, m_pCameraObject(nullptr)
					, m_TotalYaw(0)
					, m_TotalPitch(0)
{}

void RotateCameraCommand::Execute()
{
	if (m_pCameraObject == nullptr)
		SetCameraObject();

	Rotate();
}

void RotateCameraCommand::Rotate()
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	const float elapsedSec{ Locator::GetTimerService()->GetElapsedTime() };

	TransformComponent* pTransformComponent{ m_pCameraObject->GetComponent<TransformComponent>() };
	
	DirectX::XMFLOAT4 currentRotation{ pTransformComponent->GetWorldRotationEuler() };
	DirectX::XMFLOAT3 newRotation{};
	newRotation.x = currentRotation.x + (m_MouseMovement.y * m_RotatingSpeed * elapsedSec);
	newRotation.y = currentRotation.y + (m_MouseMovement.x * m_RotatingSpeed * elapsedSec);
	newRotation.z = currentRotation.z;

	pTransformComponent->Rotate(newRotation, true);
}

void RotateCameraCommand::SetCameraObject()
{
	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	Scene* pScene{ pSceneManager->GetActiveGameScene() };
	m_pCameraObject = pScene->GetSceneCamera();
}