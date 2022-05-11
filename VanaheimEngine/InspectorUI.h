#pragma once
#include "UI.h"
#include <string>

class GameObject;
class InspectorUI final : public UI
{
	public:
		InspectorUI();
		~InspectorUI();

		InspectorUI(const InspectorUI&) = delete;
		InspectorUI& operator=(const InspectorUI&) = delete;
		InspectorUI(InspectorUI&&) = delete;
		InspectorUI& operator=(InspectorUI&&) = delete;

		void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }

	protected:
		void Initialize() override;
		void ShowWindow() override;
	
	private:
		GameObject* m_pGameObject;
		
		void Draw();
		bool DrawXMFlOAT2Controlls(const std::string& label, float* values, const float columnWidth = 100.f, const float resetValue = 0.f, const float min = 0.f, const float max = 0.f);
		bool DrawXMFlOAT3Controlls(const std::string& label, float* values, const float columnWidth = 100.f, const float resetValue = 0.f, const float min = 0.f, const float max = 0.f);
		bool DrawXMINT2Controlls(const std::string& label, int* values, const float columnWidth = 100.f, const int resetValue = 0, const int min = 0.f, const int max = 0.f);
		bool DrawXMINT3Controlls(const std::string& label, int* values, const float columnWidth = 100.f, const int resetValue = 10, const int min = 0.f, const int max = 0.f);
		void DrawComponents();
		void AddComponent();

		template<typename T, typename UIFunction>
		void DrawSingleComponent(const std::string& name, UIFunction uiFunction);
};

template<typename T, typename UIFunction>
inline void InspectorUI::DrawSingleComponent(const std::string& name, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags{ ImGuiTreeNodeFlags_DefaultOpen | 
											ImGuiTreeNodeFlags_AllowItemOverlap | 
											ImGuiTreeNodeFlags_Framed |
											ImGuiTreeNodeFlags_SpanAvailWidth |
											ImGuiTreeNodeFlags_FramePadding };
	if (m_pGameObject->HasComponent<T>())
	{
		// Get the component of the gameobject
		T* pComponent{m_pGameObject->GetComponent<T>()};

		// Retrieve the current available content region
		ImVec2 contentRegionAvailable{ ImGui::GetContentRegionAvail() };

		// Set the style for the Tree node
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		const float lineHeight{ GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f };
		ImGui::Separator();
		bool open{ ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str()) };
		ImGui::PopStyleVar();
		
		// Button
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent{ false };
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(pComponent);
			ImGui::TreePop();
		}
		
		if (removeComponent)
			m_pGameObject->RemoveComponent<T>();
	}
}