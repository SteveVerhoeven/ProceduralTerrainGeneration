// >----------------------------------< //
// > Copyright 2021 - Vanaheim Engine < //
// > Author: Steve Verhoeven		  < //
// >----------------------------------< //
#include "pch.h"
#include "VanaheimEngine.h"

// Vanaheim includes
#include "UIManager.h"
#include "Timer.h"
#include "Window.h"
#include "Graphics.h"
#include "Locator.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "GeneratorManager.h"

VanaheimEngine::VanaheimEngine()
			   : m_pTimer(nullptr)
			   , m_pWindow(nullptr)
			   , m_pGraphics(nullptr)
			   , m_pUIManager(nullptr)
			   , m_pDebugLogger(nullptr)
			   , m_pSceneManager(nullptr)
			   , m_pInputManager(nullptr)
			   , m_pResourceManager(nullptr)
			   , m_pGeneratorManager(nullptr)
{}
VanaheimEngine::~VanaheimEngine()
{
	DELETE_POINTER(m_pTimer);
	DELETE_POINTER(m_pWindow);
	DELETE_POINTER(m_pGraphics);
	DELETE_POINTER(m_pUIManager);
	DELETE_POINTER(m_pDebugLogger);
	DELETE_POINTER(m_pSceneManager);
	DELETE_POINTER(m_pInputManager);
	DELETE_POINTER(m_pResourceManager);
	DELETE_POINTER(m_pGeneratorManager);
}

void VanaheimEngine::Initialize(HINSTANCE instance)
{
	int width{ 2560 }, height{ 1080 };

	m_pWindow = new Window("Vanaheim Engine", width, height, instance);
	m_pGraphics = new Graphics(m_pWindow->GetWindowHandle(), width, height);

	InitializeLocator();
}
void VanaheimEngine::Update()
{
	m_pTimer->Update();
}

void VanaheimEngine::InitializeLocator()
{
	Locator::ProvideGraphicsService(m_pGraphics);
	Locator::ProvideWindowService(m_pWindow);
	
	m_pUIManager = new UIManager(m_pWindow, m_pGraphics);
	Locator::ProvideUIManagerService(m_pUIManager);

	m_pTimer = new Timer();
	Locator::ProvideTimerService(m_pTimer);

	m_pDebugLogger = new DebugLogger();
	m_pDebugLogger->Initialize();
	Locator::ProvideLoggingService(m_pDebugLogger);

	m_pSceneManager = new SceneManager();
	Locator::ProvideSceneService(m_pSceneManager);

	m_pInputManager = new InputManager();
	Locator::ProvideInputService(m_pInputManager);

	m_pResourceManager = new ResourceManager();
	Locator::ProvideResourceService(m_pResourceManager);

	m_pGeneratorManager = new GeneratorManager();
	Locator::ProvideGeneratorManagerService(m_pGeneratorManager);
}

void VanaheimEngine::Render()
{
	m_pGraphics->ClearBackbuffer();
	Locator::GetSceneManagerService()->Render();
	m_pGraphics->PresentBackbuffer();
}