#include "pch.h"
#include "UI.h"

// Vanaheim Includes

UI::UI(const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size)
   : m_Name(windowName)
   , m_Position(position)
   , m_Size(size)
   , m_RenderUI(true)
{}

void UI::CreateWindowBase()
{
	/*ImGui::SetNextWindowPos(ImVec2(m_Position.x, m_Position.y));
	ImGui::SetNextWindowSize(ImVec2(m_Size.x, m_Size.y));*/
	//ImGuiWindowFlags window_flags = 0;
	ImGui::Begin(m_Name.c_str(), &m_RenderUI/*, window_flags*/);
	//ImGui::PushAllowKeyboardFocus(false);	
	//ImGui::SetWindowFocus();
}

void UI::EndWindowBase()
{
	//End
	//ImGui::PopAllowKeyboardFocus();
	ImGui::End();
}