#include "pch.h"
#include "Scene_Game.h"

#include <InputManager.h>
#include <MoveCameraCommand.h>
#include <RotateCameraCommand.h>

#include <UIManager.h>
#include <InspectorUI.h>
#include <Material_ProcGen.h>
#include <TerrainGenerator.h>
#include <GeneratorManager.h>
#include <ResourceManager.h>

#include "NoiseGenerator.h"
#include "TerrainGenerator.h"

Scene_Game::Scene_Game()
		   : Scene()
		   , m_NoiseGenSettings(true, 0, 0, 0.f, 0.f, 0.f, { 0, 0 })
		   , m_ProcGenSettings(0, 0, m_NoiseGenSettings)
{}
Scene_Game::~Scene_Game()
{}

void Scene_Game::Initialize()
{
	CreateMainCamera();
	CreateLandscape();

	// Inputs
	CreateInputs();

	ActivateScene();
	Scene::Initialize();
}
void Scene_Game::PostInitialize()
{ Scene::PostInitialize(); }
void Scene_Game::Update(const float elapsedSec)
{ Scene::Update(elapsedSec); }
void Scene_Game::FixedUpdate(const float timeEachUpdate)
{ Scene::FixedUpdate(timeEachUpdate); }
void Scene_Game::LateUpdate()
{ Scene::LateUpdate(); }
void Scene_Game::Render() const
{ Scene::Render(); }

void Scene_Game::CreateInputs()
{
	InputManager* pInputManager{ Locator::GetInputManagerService() };

	pInputManager->AddKeyToMap(ControllerButton::ButtonUp, KeyboardButton::W, ButtonPressType::BUTTON_RELEASED, "MOVE_FORWARD", new MoveCameraCommand(true, false, false, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonDown, KeyboardButton::S, ButtonPressType::BUTTON_RELEASED, "MOVE_BACKWARD", new MoveCameraCommand(false, true, false, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonLeft, KeyboardButton::A, ButtonPressType::BUTTON_RELEASED, "MOVE_LEFT", new MoveCameraCommand(false, false, true, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonRight, KeyboardButton::D, ButtonPressType::BUTTON_RELEASED, "MOVE_RIGHT", new MoveCameraCommand(false, false, false, true));
	pInputManager->AddKeyToMap(ControllerButton::ButtonLThumbStick, KeyboardButton::E, ButtonPressType::BUTTON_HOLD, "ROTATE", new RotateCameraCommand());
}
void Scene_Game::CreateMainCamera()
{
	// **********************
	// Creating - Main camera
	// **********************
	const std::string name3{ "Camera-Main" };
	const DirectX::XMFLOAT3 pos3{ 300, 50, -650 };
	CreateCamera(name3, pos3);
}
void Scene_Game::CreateLandscape()
{
	// Generate landscape	
	GeneratorManager* pGeneratorManager{ Locator::GetGeneratorManagerService() };
	TerrainGenerator* pProcGen(pGeneratorManager->GetGenerator<TerrainGenerator>());
	Mesh* pMesh{ pProcGen->CreateTerrain() };

	// Model
	ResourceManager* pResourceManager{ Locator::GetResourceManagerService() };
	Texture* pNormalTexture{ pResourceManager->LoadTexture("./Resources/Textures/Landscape/noiseMap.bmp") };
	Texture* pColorTexture{ pResourceManager->LoadTexture("./Resources/Textures/Landscape/colorMap.bmp") };

	UIManager* pUIManager{ Locator::GetUIManagerService() };
	InspectorUI* pVanaheimUI{ pUIManager->GetUI<InspectorUI>() };	
	if (pVanaheimUI)
	{
		pVanaheimUI->AddObserver(pNormalTexture);
		pVanaheimUI->AddObserver(pColorTexture);
	}

	Material_ProcGen* pMaterial = new Material_ProcGen();
	pMaterial->AddTexture(pNormalTexture);
	pMaterial->AddTexture(pColorTexture);

	const std::string name{ "Landscape" };
	const DirectX::XMFLOAT3 pos{ 0, -1, 0 };
	Create3DObject(name, pos, pMesh, pMaterial);
}