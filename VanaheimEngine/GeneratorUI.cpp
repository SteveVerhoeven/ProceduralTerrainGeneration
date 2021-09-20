#include "pch.h"
#include "GeneratorUI.h"

// Vanaheim Includes
#include "GeneratorManager.h"
#include "NoiseGenerator.h"
#include "TerrainGenerator.h"

GeneratorUI::GeneratorUI(const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size)
		    : UI(windowName, position, size)
{}

void GeneratorUI::ShowWindow()
{
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2(m_Position.x, m_Position.y));
	ImGui::SetNextWindowSize(ImVec2(m_Size.x, m_Size.y));
	ImGuiWindowFlags window_flags = 0;

	ImGui::Begin(m_Name.c_str(), &windowActive, window_flags);
	ImGui::PushAllowKeyboardFocus(false);
	ImGui::SetWindowFocus();

	ImGui::Text("VARIABLES");
	ImGui::Indent();
	ImGui::PushButtonRepeat(true);
	DisplayVariables();
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

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
}

void GeneratorUI::DisplayVariables()
{
	size_t nbrOfVars{ m_Variables.size() };
	for (size_t i{}; i < nbrOfVars; ++i)
	{
		GeneratorVariable var{ m_Variables[i] };
		if (var.buttonType == UIButtonType::LIST)
		{
			/*GeneratorManager* pGenManager{ Locator::GetGeneratorManagerService() };
			TerrainGenerator* pProcGen{ pGenManager->GetGenerator<TerrainGenerator>() };

			UseListBox(var.varName, (int)pProcGen->GetProcGenSettings().type, var.varEvents);*/
		}
		else if (var.buttonType == UIButtonType::ARROW_BUTTON)
		{
			UseArrowButton(var.varName, var.varEvents, var.generatorType);
		}
		else if (var.buttonType == UIButtonType::SLIDER_FLOAT)
		{
			UseSliderFloat(var.varName, var.generatorType, var.varRange, var.varEvents);
		}
		else if (var.buttonType == UIButtonType::SLIDER_INT)
		{
			UseSliderInt(var.varName, var.generatorType, var.varRange, var.varEvents);
		}
	}
}

void GeneratorUI::UseArrowButton(const std::string& name, const std::vector<ObserverEvent>& events, const GeneratorType& genType)
{
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::Text(name.c_str());
	ImGui::SameLine(0.0f, spacing);

	bool rebuildLandscape{ DisplayArrows(name, events) };
	if (genType == GeneratorType::NOISE)
		DisplayNoiseGenVariableValue(name);
	else
		DisplayProcGenVariableValue(name);

	if (rebuildLandscape)
		Notify(ObserverEvent::REBUILD_LANDSCAPE);
}
void GeneratorUI::UseListBox(const std::string& name, int type, const std::vector<ObserverEvent>& events)
{
	const char* items[]{ "NONE", "CIRCULAR", "ROUNDEDSQUARE" };
	if (ImGui::ListBox(name.c_str(), &type, items, IM_ARRAYSIZE(items)))
	{
		Notify(events[type]);

		Notify(ObserverEvent::REBUILD_LANDSCAPE);
		return;
	}
}
void GeneratorUI::UseSliderFloat(const std::string& name, const GeneratorType& genType, const DirectX::XMFLOAT2& range, const std::vector<ObserverEvent>& events)
{
	GeneratorManager* pGeneratorManager{ Locator::GetGeneratorManagerService() };

	float value{};
	if (genType == GeneratorType::NOISE)
		value = pGeneratorManager->GetGenerator<NoiseGenerator>()->GetValueByName<float>(name);
	else
		value = pGeneratorManager->GetGenerator<TerrainGenerator>()->GetValueByName<float>(name);

	const float OldValue{ value };
	if (ImGui::SliderFloat(name.c_str(), &value, range.x, range.y, "ratio = %.1f"))
	{
		if (OldValue < value)
			Notify(events[1]);
		else if (value < OldValue)
			Notify(events[0]);

		Notify(ObserverEvent::REBUILD_LANDSCAPE);
	}
}
void GeneratorUI::UseSliderInt(const std::string& name, const GeneratorType& genType, const DirectX::XMFLOAT2& range, const std::vector<ObserverEvent>& events)
{
	GeneratorManager* pGeneratorManager{ Locator::GetGeneratorManagerService() };

	int value{};
	if (genType == GeneratorType::NOISE)
		value = pGeneratorManager->GetGenerator<NoiseGenerator>()->GetValueByName<int>(name);
	else
		value = pGeneratorManager->GetGenerator<TerrainGenerator>()->GetValueByName<int>(name);
	
	const int OldValue{ value };
	if (ImGui::SliderInt(name.c_str(), &value, (int)range.x, (int)range.y, "%d"))
	{
		if (OldValue < value)
			Notify(events[1]);
		else if (value < OldValue)
			Notify(events[0]);

		Notify(ObserverEvent::REBUILD_LANDSCAPE);
	}
}

bool GeneratorUI::DisplayArrows(const std::string& name, const std::vector<ObserverEvent>& events)
{
	bool rebuildLandscape{ false };
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	const std::string arrowButtonLeft{ "##left_" + name };
	const std::string arrowButtonRight{ "##right_" + name };
	if (ImGui::ArrowButton(arrowButtonLeft.c_str(), ImGuiDir_Left))
	{
		Notify(events[0]);
		rebuildLandscape = true;
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton(arrowButtonRight.c_str(), ImGuiDir_Right))
	{
		Notify(events[1]);
		rebuildLandscape = true;
	}
	ImGui::SameLine();

	return rebuildLandscape;
}

void GeneratorUI::DisplayNoiseGenVariableValue(const std::string& /*name*/)
{
	/*GeneratorManager* pGenManager{ Locator::GetGeneratorManagerService() };
	Noise* pNoiseGen{ pGenManager->GetGenerator<Noise>() };
	NoiseGenSettings settings{ pNoiseGen->GetNoiseGenSettings() };

	if (name == "Octaves")
		ImGui::Text("%d", settings.octaves);
	else if (name == "Lacunarity")
		ImGui::Text("%.2f", settings.lacunarity);
	else if (name == "Persistence")
		ImGui::Text("%.2f", settings.persistence);*/
}
void GeneratorUI::DisplayProcGenVariableValue(const std::string& /*name*/)
{
	/*GeneratorManager* pGenManager{ Locator::GetGeneratorManagerService() };
	TerrainGenerator* pProcGen{ pGenManager->GetGenerator<TerrainGenerator>() };
	ProcGenSettings settings{ pProcGen->GetProcGenSettings() };

	if (name == "xRes")
		ImGui::Text("%d", settings.xRes);
	else if (name == "zRes")
		ImGui::Text("%d", settings.zRes);
	else if (name == "fallOfSize")
		ImGui::Text("%.2f", settings.fallOfSize);
	else if (name == "seaLevel")
		ImGui::Text("%.2f", settings.seaLevel);
	else if (name == "type")
		ImGui::Text("%d", settings.type);*/
}