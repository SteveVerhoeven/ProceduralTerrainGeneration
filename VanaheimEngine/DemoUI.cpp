#include "VanaheimPCH.h"
#include "DemoUI.h"

DemoUI::DemoUI(const bool showDemo, const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size)
	   : UI(windowName, position, size)
	   , m_ShowDemo(showDemo)
{}

void DemoUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	CreateWindowBase();
	ImGui::ShowDemoWindow(&m_ShowDemo);
	EndWindowBase();
}