#include "pch.h"
#include "DemoUI.h"

DemoUI::DemoUI(const bool showDemo, const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size)
	   : UI(windowName, position, size)
	   , m_ShowDemo(showDemo)
{}

void DemoUI::ShowWindow()
{
	ImGui::ShowDemoWindow(&m_ShowDemo);
}