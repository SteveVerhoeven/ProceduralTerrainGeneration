// >----------------------------------< //
// > Copyright 2021 - Vanaheim Engine < //
// > Author: Steve Verhoeven		  < //
// >----------------------------------< //
#include "VanaheimPCH.h"
#include "VanaheimEngine.h"

// Vanaheim includes
#include "Timer.h"
#include "Window.h"
#include "Graphics.h"

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
	m_pWindow = new Window("Vanaheim Engine", instance);
	m_pGraphics = new Graphics(m_pWindow);

	InitializeLocator();
	InitializeEngineUI();
}
void VanaheimEngine::Update()
{
	m_pTimer->Update();
}

void VanaheimEngine::InitializeLocator()
{
	Locator::ProvideGraphicsService(m_pGraphics);
	Locator::ProvideWindowService(m_pWindow);
	
	m_pUIManager = new UIManager(m_pWindow);
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
void VanaheimEngine::InitializeEngineUI()
{
	InspectorUI* pInspectorUI{ new InspectorUI() };
	m_pUIManager->AddUI(pInspectorUI);

	ConsoleUI* pConsoleUI{ new ConsoleUI() };
	m_pUIManager->AddUI(pConsoleUI);

	ViewportUI* pViewportUI{ new ViewportUI() };
	m_pUIManager->AddUI(pViewportUI);

	HierarchyUI* pHierarchyUI{ new HierarchyUI() };
	m_pUIManager->AddUI(pHierarchyUI);
}

void VanaheimEngine::Render()
{
	m_pGraphics->ClearBackbuffer();
	Locator::GetSceneManagerService()->Render();
	m_pGraphics->PresentBackbuffer();
}