#include "VanaheimPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Graphics.h"

SceneManager::SceneManager()
			 : m_pGameScenes(std::vector<Scene*>())
			 , m_pMenuScenes(std::vector<Scene*>())
			 , m_ActiveGameSceneIndex()
			 , m_ActiveMenuSceneIndex()
			 , m_MenuActive(true)
			 , m_GameActive(false)
			 , m_pGraphics(nullptr)
			 , m_pUIManager(nullptr)
{}
SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pMenuScenes)
		DELETE_POINTER(pScene);

	for (Scene* pScene : m_pGameScenes)
		DELETE_POINTER(pScene);

	m_pMenuScenes.clear();
	m_pGameScenes.clear();
}

void SceneManager::Initialize()
{
	m_pGraphics = Locator::GetGraphicsService();
	m_pUIManager = Locator::GetUIManagerService();

	for (Scene* pScene : m_pMenuScenes)
		pScene->Initialize();

	for (Scene* pScene : m_pGameScenes)
		pScene->Initialize();
}
void SceneManager::PostInitialize()
{
	for (Scene* pScene : m_pMenuScenes)
		pScene->PostInitialize();

	for (Scene* pScene : m_pGameScenes)
		pScene->PostInitialize();
}
void SceneManager::Update(const float elapsedSec)
{
	for (Scene* pScene : m_pMenuScenes)
		pScene->Update(elapsedSec);

	for (Scene* pScene : m_pGameScenes)
		pScene->Update(elapsedSec);
}
void SceneManager::FixedUpdate(const float timeEachUpdate)
{
	for (Scene* pScene : m_pMenuScenes)
		pScene->FixedUpdate(timeEachUpdate);

	for (Scene* pScene : m_pGameScenes)
		pScene->FixedUpdate(timeEachUpdate);
}
void SceneManager::LateUpdate()
{
	for (Scene* pScene : m_pMenuScenes)
		pScene->LateUpdate();

	for (Scene* pScene : m_pGameScenes)
		pScene->LateUpdate();
}
void SceneManager::Render()
{	
	m_pGraphics->ClearBackbuffer();
	m_pUIManager->BeginFrame();
	
	m_pGraphics->SetGameRenderTarget();		// Set the render target for the viewport in the UI
	for (Scene* pScene : m_pMenuScenes)
		pScene->Render();

	for (Scene* pScene : m_pGameScenes)
		pScene->Render();
	
	m_pGraphics->SetMainRenderTarget();		// Set the render target for the entire program window
	m_pUIManager->Render();

	m_pUIManager->EndFrame();
	m_pGraphics->PresentBackbuffer();
}

void SceneManager::DeactivateAllGameScenes()
{
	for (Scene* pScene : m_pGameScenes)
		pScene->DeactivateScene();

	m_GameActive = false;
}
void SceneManager::DeactivateAllMenuScenes()
{
	for (Scene* pScene : m_pMenuScenes)
		pScene->DeactivateScene();

	m_MenuActive = false;
}

void SceneManager::ActivateNextGameScene()
{
	// Deactivate current scene
	m_pGameScenes[size_t(m_ActiveGameSceneIndex)]->DeactivateScene();

	// Increment index that remembers which scene the game is on
	m_ActiveGameSceneIndex++;

	// Activate the new current scene
	m_pGameScenes[size_t(m_ActiveGameSceneIndex)]->ActivateScene();
}

void SceneManager::ActivateGameSceneByIndex(const size_t sceneIndex)
{
	m_pGameScenes[sceneIndex]->ActivateScene();
	m_ActiveGameSceneIndex = int(sceneIndex);

	m_GameActive = true;
}
void SceneManager::ActivateMenuSceneByIndex(const size_t sceneIndex)
{
	m_pMenuScenes[sceneIndex]->ActivateScene();
	m_ActiveMenuSceneIndex = int(sceneIndex);

	m_MenuActive = true;
}