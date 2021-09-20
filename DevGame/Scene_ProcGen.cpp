#include "pch.h"
#include "Scene_ProcGen.h"

#include <InputManager.h>
#include <MoveCameraCommand.h>
#include <RotateCameraCommand.h>

#include <UIManager.h>
#include <GeneratorUI.h>
#include <Material_ProcGen.h>
#include <TerrainGenerator.h>
#include <GeneratorManager.h>
#include <ResourceManager.h>

#include "NoiseGenerator.h"
#include "TerrainGenerator.h"

Scene_ProcGen::Scene_ProcGen()
	: Scene()
	, m_NoiseGenSettings(0, 0, 0.f, 0.f, 0.f, { 0, 0 })
	, m_ProcGenSettings(0, 0, m_NoiseGenSettings)
{}
Scene_ProcGen::~Scene_ProcGen()
{}

void Scene_ProcGen::Initialize()
{
	CreateMainCamera();
	CreateLandscape();

	// Inputs
	CreateInputs();

	GeneratorManager* pGeneratorManager{ Locator::GetGeneratorManagerService() };
	GeneratorUI* pGeneratorUI{ Locator::GetUIManagerService()->GetUI<GeneratorUI>() };
	pGeneratorUI->AddObserver(pGeneratorManager->GetGenerator<NoiseGenerator>());
	pGeneratorUI->AddObserver(pGeneratorManager->GetGenerator<TerrainGenerator>());

	ActivateScene();
	Scene::Initialize();
}
void Scene_ProcGen::PostInitialize()
{
	Scene::PostInitialize();
}
void Scene_ProcGen::Update(const float elapsedSec)
{
	Scene::Update(elapsedSec);
}
void Scene_ProcGen::FixedUpdate(const float timeEachUpdate)
{
	Scene::FixedUpdate(timeEachUpdate);
}
void Scene_ProcGen::LateUpdate()
{
	Scene::LateUpdate();
}
void Scene_ProcGen::Render() const
{
	Scene::Render();
}

void Scene_ProcGen::CreateInputs()
{
	InputManager* pInputManager{ Locator::GetInputManagerService() };

	pInputManager->AddKeyToMap(ControllerButton::ButtonUp, KeyboardButton::W, ButtonPressType::BUTTON_RELEASED, "MOVE_FORWARD", new MoveCameraCommand(true, false, false, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonDown, KeyboardButton::S, ButtonPressType::BUTTON_RELEASED, "MOVE_BACKWARD", new MoveCameraCommand(false, true, false, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonLeft, KeyboardButton::A, ButtonPressType::BUTTON_RELEASED, "MOVE_LEFT", new MoveCameraCommand(false, false, true, false));
	pInputManager->AddKeyToMap(ControllerButton::ButtonRight, KeyboardButton::D, ButtonPressType::BUTTON_RELEASED, "MOVE_RIGHT", new MoveCameraCommand(false, false, false, true));
	pInputManager->AddKeyToMap(ControllerButton::ButtonLThumbStick, KeyboardButton::E, ButtonPressType::BUTTON_HOLD, "ROTATE", new RotateCameraCommand());
}
void Scene_ProcGen::CreateMainCamera()
{
	// **********************
	// Creating - Main camera
	// **********************
	const std::string name3{ "Camera-Main" };
	const DirectX::XMFLOAT3 pos3{ 200, 100, -600 };
	CreateCamera(name3, pos3);
}
void Scene_ProcGen::CreateLandscape()
{
	// **********************
	// Creating - Landscape
	// **********************
	const int size{ 400 };
	// Noise Generator settings
	const size_t seed{ 1 };
	const size_t octaves{ 5 };
	const float lacunarity{ 2.3f };
	const float persistence{ 0.448f };
	const float scale{ 32.24f };
	m_NoiseGenSettings = NoiseGenSettings{ seed, octaves, scale, lacunarity, persistence, {size, size} };

	// Procedural Landscape Generator settings
	const int xRes{ size };
	const int zRes{ size };
	m_ProcGenSettings = ProcGenSettings{ xRes, zRes, m_NoiseGenSettings };

	// Generate landscape	
	GeneratorManager* pGeneratorManager{ Locator::GetGeneratorManagerService() };
	TerrainGenerator* pProcGen(pGeneratorManager->GetGenerator<TerrainGenerator>());
	pProcGen->EditSettings(m_ProcGenSettings);
	Mesh* pMesh{ pProcGen->CreateTerrain() };

	// Model
	ResourceManager* pResourceManager{ Locator::GetResourceManagerService() };
	Texture* pNormalTexture{ pResourceManager->LoadTexture("./Resources/Textures/Landscape/noiseMap.bmp") };
	Texture* pColorTexture{ pResourceManager->LoadTexture("./Resources/Textures/Landscape/colorMap.bmp") };

	UIManager* pUIManager{ Locator::GetUIManagerService() };
	GeneratorUI* pGeneratorUI{ pUIManager->GetUI<GeneratorUI>() };
	pGeneratorUI->AddObserver(pNormalTexture);
	pGeneratorUI->AddObserver(pColorTexture);

	Material_ProcGen* pMaterial = new Material_ProcGen();
	pMaterial->AddTexture(pNormalTexture);
	pMaterial->AddTexture(pColorTexture);

	const std::string name{ "Landscape" };
	const DirectX::XMFLOAT3 pos{ 0, -1, 0 };
	Create3DObject(name, pos, pMesh, pMaterial);
}