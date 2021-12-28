#include "pch.h"
#include "InspectorUI.h"

#include "Window.h"

// Vanaheim Includes
#include "GeneratorManager.h"
#include "NoiseGenerator.h"
#include "TerrainGenerator.h"
#include "UIData.h"

InspectorUI::InspectorUI()
		   : UI("Inspector", DirectX::XMFLOAT2{ 0.f, 0.f }, DirectX::XMFLOAT2{ 0.f, 0.f })
{
	SetPositionAndSize();
}
InspectorUI::~InspectorUI()
{
	DELETE_POINTERS(m_Variables, m_Variables.size());
}

void InspectorUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	CreateWindowBase();

	ImGui::Text("VARIABLES");
	ImGui::Indent();
	ImGui::PushButtonRepeat(true);
	{
		DisplayInspectorVariables();
	}
	ImGui::Unindent();
	ImGui::PopButtonRepeat();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	EndWindowBase();
}

void InspectorUI::SetPositionAndSize()
{
	Window* pWindow{ Locator::GetWindowService() };
	const float width{ pWindow->GetWindowWidth() / 100.f * 20.f };
	const float x{ pWindow->GetWindowWidth() - width };
	m_Position = DirectX::XMFLOAT2(x, 0);
	m_Size = DirectX::XMFLOAT2(width, float(pWindow->GetWindowHeight()));
}

void InspectorUI::DisplayInspectorVariables()
{
	const size_t nbrOfVariables{ m_Variables.size() };
	for (size_t i{}; i < nbrOfVariables; ++i)
	{
		InspectorVariable* variable{ m_Variables[i] };
		FInspectorVariable* varF{ dynamic_cast<FInspectorVariable*>(variable) };
		if (varF)
		{
			if (ImGui::SliderFloat(varF->name.c_str(), varF->value, varF->varRange.x, varF->varRange.y, "ratio = %.1f"))
				Notify(ObserverEvent::REBUILD_LANDSCAPE);
		}
		else
		{
			STInspectorVariable* varST{ dynamic_cast<STInspectorVariable*>(variable) };
			if (ImGui::SliderInt(varST->name.c_str(), (int*)varST->value, int(varST->varRange.x), int(varST->varRange.y), "ratio = %d"))
				Notify(ObserverEvent::REBUILD_LANDSCAPE);
		}
		
	}
}
