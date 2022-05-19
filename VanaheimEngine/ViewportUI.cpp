#include "VanaheimPCH.h"
#include "ViewportUI.h"

#include "Graphics.h"
#include "Window.h"

ViewportUI::ViewportUI()
		   : UI("Viewport", DirectX::XMFLOAT2{ 0.f, 0.f }, DirectX::XMFLOAT2{ 0.f, 0.f })
{}
ViewportUI::~ViewportUI()
{}

void ViewportUI::Initialize()
{}
void ViewportUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	BeginWindowBase();

	Graphics* pGraphics{ Locator::GetGraphicsService() };
	const float width{ ImGui::GetWindowWidth() };
	const float height{ ImGui::GetWindowHeight() };
	ImGui::Image(pGraphics->GetShaderResourceView(), ImVec2{width, height});

	EndWindowBase();
}