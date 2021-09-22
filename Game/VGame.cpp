#include "pch.h"
#include "VGame.h"

#include <SceneManager.h>
#include "Scene_Game.h"

VGame::VGame(const std::string& name)
	: Game(name)
{}

void VGame::Initialize(HINSTANCE hInstance)
{
	Game::Initialize(hInstance);

	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	Scene_Game* pScene_ProcGen = new Scene_Game();
	pSceneManager->AddGameScene(pScene_ProcGen);
	pSceneManager->Initialize();
}
void VGame::PostInitialize()
{
	SceneManager* pSceneManager{ Locator::GetSceneManagerService() };
	pSceneManager->PostInitialize();
}