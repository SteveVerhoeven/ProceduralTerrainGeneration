#include "pch.h"
#include "ConsoleUI.h"

#include "Window.h"
#include "UIManager.h"
#include "InspectorUI.h"

ConsoleUI::ConsoleUI()
		  : UI("Console", DirectX::XMFLOAT2{ 0.f, 0.f }, DirectX::XMFLOAT2{ 0.f, 0.f })
		  , m_ConsoleMessages(/*std::vector<std::string>()*/)
{
	SetPositionAndSize();

	BInspectorVariable* InspectorVarRenderUI{ new BInspectorVariable() };
	InspectorVarRenderUI->name = "Show Console";
	InspectorVarRenderUI->value = &m_RenderUI;
	InspectorVarRenderUI->varRange = { 0, 0 };
	UIManager* pUIManager{ Locator::GetUIManagerService() };
	InspectorUI* pVanaheimUI{ pUIManager->GetUI<InspectorUI>() };
	if (pVanaheimUI)
	{
		pVanaheimUI->StoreVariable(InspectorVarRenderUI);
	}
}

void ConsoleUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	CreateWindowBase();

	ImGui::Indent();
	ImGui::PushButtonRepeat(true);
	{
		//const size_t nbrOfConsoleMessages{ m_ConsoleMessages.size() };
		const size_t nbrOfConsoleMessages{ m_MaxConsoleSize };
		for (size_t i = 0; i < nbrOfConsoleMessages; ++i)
		{
			ImGui::Text(m_ConsoleMessages[i].c_str());
		}
	}
	ImGui::PopButtonRepeat();

	EndWindowBase();
}

void ConsoleUI::SetPositionAndSize()
{
	Window* pWindow{ Locator::GetWindowService() };
	const float width_Inspector{ pWindow->GetWindowWidth() / 100.f * 20.f };
	const float height_Console{ pWindow->GetWindowHeight() / 100.f * 25.f };
	const float y{ pWindow->GetWindowHeight() - height_Console };
	m_Position = DirectX::XMFLOAT2(0, y);
	m_Size = DirectX::XMFLOAT2(float(pWindow->GetWindowWidth()) - width_Inspector, height_Console);
}
