#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "UIManager.h"
#include "GeneratorUI.h"
#include "DemoUI.h"
#include "Graphics.h"

SceneManager::SceneManager()
			 : m_pGameScenes(std::vector<Scene*>())
			 , m_pMenuScenes(std::vector<Scene*>())
			 , m_ActiveGameSceneIndex()
			 , m_ActiveMenuSceneIndex()
			 , m_MenuActive(true)
			 , m_GameActive(false)
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
void SceneManager::Render()
{
	Graphics* pGraphics{ Locator::GetGraphicsService() };
	pGraphics->ClearBackbuffer();	


	UIManager* pUIManager{ Locator::GetUIManagerService() };
	pUIManager->BeginFrame();
	
	for (Scene* pScene : m_pMenuScenes)
		pScene->Render();

	for (Scene* pScene : m_pGameScenes)
		pScene->Render();

	
	pUIManager->GetUI<GeneratorUI>()->ShowWindow();
	pUIManager->GetUI<DemoUI>()->ShowWindow();
	
	pUIManager->EndFrame();
	pGraphics->PresentBackbuffer();
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