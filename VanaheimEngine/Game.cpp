#include "pch.h"
#include "Game.h"

#include "Timer.h"
#include "InputManager.h"
#include "SceneManager.h"

#include "VanaheimEngine.h"

Game::Game(const std::string& name)
	 : m_Name(name)
	 , m_pEngine( new VanaheimEngine() )
{}
Game::~Game()
{
	DELETE_POINTER(m_pEngine);
}

void Game::Initialize(HINSTANCE hInstance)
{
	m_pEngine->Initialize(hInstance);
}
int Game::GameLoop()
{
	Timer* pTimer{ Locator::GetTimerService() };
	const float timeEachUpdate{ pTimer->GetMsEachUpdate() };

	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		// Update engine
		m_pEngine->Update();

		const float elapsedSec{ pTimer->GetElapsedTime() };

		// Update input
		InputManager* pInputManager{ Locator::GetInputManagerService() };
		msg = pInputManager->ProcessInput(elapsedSec);

		// Update game
		SceneManager* sceneManager{ Locator::GetSceneManagerService() };
		while (pTimer->GetLag() >= timeEachUpdate)
		{
			sceneManager->FixedUpdate(timeEachUpdate);
			pTimer->SubtractFixedUpdateFromLag();
		}
		sceneManager->Update(elapsedSec);

		// Render game
		sceneManager->Render();
	}

	return msg.wParam;
}