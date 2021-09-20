#include "pch.h"
#include "Scene_Dev.h"

#include <InputManager.h>
#include <MoveCameraCommand.h>
#include <RotateCameraCommand.h>

Scene_Dev::Scene_Dev()
		  : Scene()
{}
Scene_Dev::~Scene_Dev()
{}

void Scene_Dev::Initialize()
{
	CreateMainCamera();

	// Inputs
	CreateInputs();

	ActivateScene();
	Scene::Initialize();
}
void Scene_Dev::PostInitialize()
{ Scene::PostInitialize(); }
void Scene_Dev::Update(const float elapsedSec)
{ Scene::Update(elapsedSec); }
void Scene_Dev::FixedUpdate(const float timeEachUpdate)
{ Scene::FixedUpdate(timeEachUpdate); }
void Scene_Dev::LateUpdate()
{ Scene::LateUpdate(); }
void Scene_Dev::Render() const
{ Scene::Render(); }

void Scene_Dev::CreateInputs()
{
	InputManager* pInputManager{ Locator::GetInputManagerService() };

	pInputManager->AddKeyToMap(ControllerButton::ButtonUp		   , KeyboardButton::W, ButtonPressType::BUTTON_RELEASED, "MOVE_FORWARD" , new MoveCameraCommand(true , false, false, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonDown		   , KeyboardButton::S, ButtonPressType::BUTTON_RELEASED, "MOVE_BACKWARD", new MoveCameraCommand(false, true , false, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonLeft		   , KeyboardButton::A, ButtonPressType::BUTTON_RELEASED, "MOVE_LEFT"    , new MoveCameraCommand(false, false, true , false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonRight	   , KeyboardButton::D, ButtonPressType::BUTTON_RELEASED, "MOVE_RIGHT"   , new MoveCameraCommand(false, false, false, true ));
	pInputManager->AddKeyToMap(ControllerButton::ButtonLThumbStick , KeyboardButton::E, ButtonPressType::BUTTON_HOLD	, "ROTATE"		 , new RotateCameraCommand());
}
void Scene_Dev::CreateMainCamera()
{
	// **********************
	// Creating - Main camera
	// **********************
	const std::string name3{ "Camera-Main" };
	const DirectX::XMFLOAT3 pos3{ 200, 100, -600 };
	CreateCamera(name3, pos3);
}