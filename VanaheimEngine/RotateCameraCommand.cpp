#include "pch.h"
#include "RotateCameraCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

RotateCameraCommand::RotateCameraCommand()
					: m_RotatingSpeed(50.f)
					, m_MousePos()
					, m_pCameraObject(nullptr)
{
	SetCameraObject();
}

void RotateCameraCommand::Execute()
{
	Rotate();
}

void RotateCameraCommand::Rotate()
{
	const float elapsedSec{ Locator::GetTimerService()->GetElapsedTime() };

	TransformComponent* pTransformComponent{ m_pCameraObject->GetComponent<TransformComponent>() };
	DirectX::XMFLOAT4 currentRotation{ pTransformComponent->GetWorldRotationEuler() };

	std::cout << "Current Rotation: \n";
	std::cout << "x: " << currentRotation.x << "\n";
	std::cout << "y: " << currentRotation.y << "\n";
	std::cout << "z: " << currentRotation.z << "\n";
	std::cout << "w: " << currentRotation.w << "\n";
	
	DirectX::XMFLOAT3 newRotation{};
	newRotation.x = currentRotation.x - (m_MousePos.y * m_RotatingSpeed * elapsedSec);
	newRotation.y = currentRotation.y - (m_MousePos.x * m_RotatingSpeed * elapsedSec);
	newRotation.z = currentRotation.z;

	std::cout << "New Rotation: \n";
	std::cout << "x: " << newRotation.x << "\n";
	std::cout << "y: " << newRotation.y << "\n";
	std::cout << "z: " << newRotation.z << "\n";

	pTransformComponent->Rotate(newRotation, true);
}

void RotateCameraCommand::SetCameraObject()
{
	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	Scene* pScene{ pSceneManager->GetActiveGameScene() };
	m_pCameraObject = pScene->GetObjectByName("Camera-Main");
}