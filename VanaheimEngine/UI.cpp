#include "VanaheimPCH.h"
#include "UI.h"

UI::UI(const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size)
   : m_Name(windowName)
   , m_Position(position)
   , m_Size(size)
   , m_RenderUI(true)
{}

void UI::BeginWindowBase()
{
	ImGui::Begin(m_Name.c_str(), &m_RenderUI);
}

void UI::EndWindowBase()
{
	ImGui::End();
}