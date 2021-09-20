#include "pch.h"
#include "DevGame.h"

#include <SceneManager.h>
#include "Scene_ProcGen.h"

DevGame::DevGame(const std::string& name)
		: Game(name)
		, m_pScene_Dev(nullptr)
		, m_pScene_ProcGen(nullptr)
{}

void DevGame::Initialize(HINSTANCE hInstance)
{
	Game::Initialize(hInstance);	
	
	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	
	m_pScene_ProcGen = new Scene_ProcGen();
	pSceneManager->AddGameScene(m_pScene_ProcGen);

	pSceneManager->Initialize();
}
void DevGame::PostInitialize()
{
	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	pSceneManager->PostInitialize();
}