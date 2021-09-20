#include "pch.h"
#include "UIManager.h"

// ImGui
#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

// Vanaheim Engine
#include "Window.h"
#include "Graphics.h"
#include "UI.h"
#include "DemoUI.h"
#include "GeneratorUI.h"

UIManager::UIManager(Window* pWindow, Graphics* pGraphics)
		  : m_pUIs(std::vector<UI*>())
{
	InitializeImGui(pWindow, pGraphics);

	AddUI(new GeneratorUI("Generator UI", { 0, 0 }, { 350, 500 }));
	AddUI(new DemoUI(false, "Demo UI", { 0, 0 }, { 200, 500 }));
}
UIManager::~UIManager()
{
	ShutdownImGui();

	for (UI* pUI: m_pUIs)
		DELETE_POINTER(pUI);

	m_pUIs.clear();
}

void UIManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void UIManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::InitializeImGui(Window* pWindow, Graphics* pGraphics)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	HWND windowHandle{ pWindow->GetWindowHandle() };
	if (!ImGui_ImplWin32_Init(windowHandle))
		Locator::GetDebugLoggerService()->Log(ErrorLevel::LOG_ERROR, "UIManager::UIManager >> ImGui_ImplWin32_Init << failed");

	if (!ImGui_ImplDX11_Init(pGraphics->GetDevice(), pGraphics->GetDeviceContext()))
		Locator::GetDebugLoggerService()->Log(ErrorLevel::LOG_ERROR, "UIManager::UIManager >> ImGui_ImplDX11_Init << failed");

	ImGui::GetIO().ImeWindowHandle = windowHandle;
}
void UIManager::ShutdownImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}