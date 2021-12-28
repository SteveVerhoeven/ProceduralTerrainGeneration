#include "pch.h"
#include "ConsoleUI.h"

#include "Window.h"

ConsoleUI::ConsoleUI()
		  : UI("Console", DirectX::XMFLOAT2{ 0.f, 0.f }, DirectX::XMFLOAT2{ 0.f, 0.f })
		  , m_ConsoleMessages(std::vector<std::string>())
{
	SetPositionAndSize();
}

void ConsoleUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	CreateWindowBase();

	ImGui::Indent();
	ImGui::PushButtonRepeat(true);
	{
		const size_t nbrOfConsoleMessages{ m_ConsoleMessages.size() };
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
	const float width{ pWindow->GetWindowWidth() / 100.f * 20.f };
	const float height{ pWindow->GetWindowHeight() / 100.f * 25.f };
	const float y{ pWindow->GetWindowHeight() - height };
	m_Position = DirectX::XMFLOAT2(0, y);
	m_Size = DirectX::XMFLOAT2(float(pWindow->GetWindowWidth()) - width, float(pWindow->GetWindowHeight()) + 100.f);
}
