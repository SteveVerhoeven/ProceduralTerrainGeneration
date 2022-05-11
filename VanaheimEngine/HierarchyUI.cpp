#include "VanaheimPCH.h"
#include "HierarchyUI.h"
#include "Scene.h"

HierarchyUI::HierarchyUI()
			: UI("Hierarchy", DirectX::XMFLOAT2{ 0.f, 0.f }, DirectX::XMFLOAT2{ 0.f, 0.f })
			, m_pScene(nullptr)
			, m_SelectedGameObject(INT32_MAX)
{}
HierarchyUI::~HierarchyUI()
{}

void HierarchyUI::Initialize()
{
	m_pScene = Locator::GetSceneManagerService()->GetActiveGameScene();
	m_pInspectorUI = Locator::GetUIManagerService()->GetUI<InspectorUI>();
}
void HierarchyUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	CreateWindowBase();
	Draw();
	EndWindowBase();
}

void HierarchyUI::Draw()
{
	// Displaying all game objects
	const std::vector<GameObject*>& gameObjects{ m_pScene->GetObjects() };
	const size_t nbrOfGameObjects{ gameObjects.size() };
	for (size_t i{}; i < nbrOfGameObjects; ++i)
	{
		bool deleteGameObject{ false };

		NameComponent* pNameComponent{ gameObjects[i]->GetComponent<NameComponent>() };

		ImGuiTreeNodeFlags flags{ ((m_SelectedGameObject == i) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow };
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened{ ImGui::TreeNodeEx((void*)(i), flags, pNameComponent->GetName().c_str()) };
		
		// If an item in the hierarchy is clicked then set the selected object and push the pointer to the inspector ui
		if (ImGui::IsItemClicked())
		{
			m_SelectedGameObject = i;
			m_pInspectorUI->SetGameObject(gameObjects[i]);
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		// Right click on empty space in hierarchy
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy Game Object"))
				deleteGameObject = true;		// Delay the deletion to prevent code from breaking due to a possible to early delete

			ImGui::EndPopup();
		}

		if (deleteGameObject)
		{
			// Set the gameobject to be removed
			gameObjects[i]->SetRemoveFlag();
			// Set the scene to be cleaned
			m_pScene->SetCleanSeneFlag();
			// Set the target of the inspector UI back to nullptr
			m_pInspectorUI->SetGameObject(nullptr);
		}
	}

	// If a click happened in the window then reset the selected object and set the object in the inspector ui to nullptr
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_SelectedGameObject = INT32_MAX;
		m_pInspectorUI->SetGameObject(nullptr);
	}

	// Right click on empty space in hierarchy
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create empty Game Object"))
			m_pScene->AddGameObject(new GameObject({}, {}, {}, "Empty Game Object"));

		ImGui::EndPopup();
	}
}
