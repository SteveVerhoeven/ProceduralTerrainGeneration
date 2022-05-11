#include "VanaheimPCH.h"
#include "InspectorUI.h"

#include "Window.h"

InspectorUI::InspectorUI()
		   : UI("Inspector", DirectX::XMFLOAT2{ 0.f, 0.f }, DirectX::XMFLOAT2{ 0.f, 0.f })
		   , m_pGameObject(nullptr)
{}
InspectorUI::~InspectorUI()
{}

void InspectorUI::Initialize()
{}
void InspectorUI::ShowWindow()
{
	if (!m_RenderUI)
		return;

	CreateWindowBase();
	Draw();
	EndWindowBase();
}

void InspectorUI::Draw()
{
	if (!m_pGameObject)
		return;

	DrawComponents();
}

void InspectorUI::DrawComponents()
{
	if (m_pGameObject->HasComponent<NameComponent>())
	{
		NameComponent* pNameComponent{ m_pGameObject->GetComponent<NameComponent>() };
		const std::string& name{ pNameComponent->GetName() };

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), name.c_str());
		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			pNameComponent->SetName(std::string(buffer));
	}

	AddComponent();

	DrawSingleComponent<TransformComponent>("Transform", [this](auto* pComponent)
	{
		/** Position */
		const DirectX::XMFLOAT3& posF3{ pComponent->GetWorldPosition() };
		float pos[] = { posF3.x, posF3.y, posF3.z };

		if (DrawXMFlOAT3Controlls("Position", pos))
			pComponent->SetWorldPosition({ pos[0], pos[1], pos[2] });

		/** Rotation */
		const DirectX::XMFLOAT4& rotF4{ pComponent->GetWorldRotation() };
		float rotation[] = { DirectX::XMConvertToDegrees(rotF4.x),
							 DirectX::XMConvertToDegrees(rotF4.y),
							 DirectX::XMConvertToDegrees(rotF4.z) };

		if (DrawXMFlOAT3Controlls("Rotation", rotation))
			pComponent->SetWorldRotation({ DirectX::XMConvertToRadians(rotation[0]),
										   DirectX::XMConvertToRadians(rotation[1]),
										   DirectX::XMConvertToRadians(rotation[2]), rotF4.w });

		/** Scale */
		const DirectX::XMFLOAT3& scaleF3{ pComponent->GetWorldScale() };
		float scale[] = { scaleF3.x, scaleF3.y, scaleF3.z };

		if (DrawXMFlOAT3Controlls("Scale", scale, 100.f, 1.f))
			pComponent->SetWorldScale({ scale[0], scale[1], scale[2] });
	});
	DrawSingleComponent<CameraComponent>("Camera", [](auto* pComponent)
	{
		bool isMainCamera{ pComponent->GetIsMainCamera() };
		float nearValue{ pComponent->GetNear() };
		float farValue{ pComponent->GetFar() };
		float fovValue{ pComponent->GetFOV_Degrees() };

		const float minNearValue{ 0.001f };
		const float maxNearValue{ farValue - 0.001f };
		const float minFarValue{ nearValue + 0.001f};
		const float maxFarValue{ FLT_EPSILON };
		
		if (ImGui::Checkbox("Main camera", &isMainCamera))
			pComponent->SetIsMainCamera(isMainCamera);

		if (ImGui::DragFloat("Near", &nearValue, 0.05f, minNearValue, maxNearValue))
			pComponent->SetNear(nearValue);

		if (ImGui::DragFloat("Far", &farValue, 1.f, minFarValue, maxFarValue))
			pComponent->SetFar(farValue);

		if (ImGui::DragFloat("FOV", &fovValue, 0.5f, 0.f, 180.f))
			pComponent->SetFOV(fovValue);
	});
	DrawSingleComponent<RenderComponent>("Renderer", [](auto* pComponent)
	{
		ModelComponent* pModelComponent{ pComponent->GetParentObject()->GetComponent<ModelComponent>() };
		if (!pModelComponent)
			return;

		bool renderComponent{ pComponent->GetCanRenderComponent() };

		if (ImGui::Checkbox("Render mesh", &renderComponent))
			pComponent->SetCanRenderComponent(renderComponent);
	});
	DrawSingleComponent<TerrainGeneratorComponent>("Terrain Generator", [this](auto* pComponent)
	{
		bool rebuildLandscape{ false };

		ImGui::Spacing();
		ImGui::Text("Noise generator settings");

		int seed = pComponent->GetSeed();
		int octaves = pComponent->GetOctaves();
		float lacunarity = pComponent->GetLacunarity();
		float scale = pComponent->GetScale();
		float persistence = pComponent->GetPersistence();
		const DirectX::XMINT2 mapSizeF2{ pComponent->GetMapsize() };
		int mapSize[] = { mapSizeF2.x, mapSizeF2.y };

		if (ImGui::DragInt("Seed", &seed, 1.f, 0, 1000))
		{
			pComponent->SetSeed(seed);
			rebuildLandscape = true;
		}

		if (ImGui::DragInt("Octaves", &octaves, 1, 1, 8))
		{
			pComponent->SetOctaves(octaves);
			rebuildLandscape = true;
		}

		if (ImGui::DragFloat("Lacunarity", &lacunarity, 0.1f, 1.f, 10.f))
		{
			pComponent->SetLacunarity(lacunarity);
			rebuildLandscape = true;
		}

		if (ImGui::DragFloat("Scale", &scale, 0.1f, 1.f, 50.f))
		{
			pComponent->SetScale(scale);
			rebuildLandscape = true;
		}

		if (ImGui::DragFloat("Persistence", &persistence, 0.001f, 0.f, 1.f))
		{
			pComponent->SetPersistence(persistence);
			rebuildLandscape = true;
		}

		if (DrawXMINT2Controlls("Mapsize", mapSize, 100.f, 0, 5, 400))
		{
			if (mapSize[0] != mapSizeF2.x)
				mapSize[1] = mapSize[0];
			if (mapSize[1] != mapSizeF2.y)
				mapSize[0] = mapSize[1];

			pComponent->SetMapsize({mapSize[0], mapSize[1]});
			rebuildLandscape = true;
		}

		if (ImGui::Button("GenerateTerrain"))
		{
			pComponent->GenerateTerrain();
		}

		if (rebuildLandscape)
		{
			Notify(ObserverEvent::REBUILD_LANDSCAPE);
		}
	});
}
void InspectorUI::AddComponent()
{
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add component"))
		ImGui::OpenPopup("Add component");

	if (ImGui::BeginPopup("Add component"))
	{
		// TODO: WIP
		/*if (ImGui::MenuItem("Camera"))
		{
			m_pGameObject->AddComponent(new CameraComponent());
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Terrain generator"))
		{
			m_pGameObject->AddComponent(new ModelComponent());
			m_pGameObject->AddComponent(new TerrainGeneratorComponent());
			ImGui::CloseCurrentPopup();
		}*/

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();
}

bool InspectorUI::DrawXMFlOAT2Controlls(const std::string& label, float* values, const float columnWidth, const float resetValue, const float min, const float max)
{
	ImGuiIO& io{ ImGui::GetIO() };
	ImFont* pBoldFont{ io.Fonts->Fonts[0] };

	ImGui::PushID(label.c_str());

	bool valueChanged{ false };
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
	ImVec2 buttonSize{ lineHeight + 3.f, lineHeight };


	/** X value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values[0] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &values[0], 0.1f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);

	/** X value - End */
	/** Y value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values[1] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &values[1], 0.1f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);
	/** Y value - End */

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return valueChanged;
}
bool InspectorUI::DrawXMFlOAT3Controlls(const std::string& label, float* values, const float columnWidth, const float resetValue, const float min, const float max)
{
	ImGuiIO& io{ ImGui::GetIO() };
	ImFont* pBoldFont{ io.Fonts->Fonts[0] };

	ImGui::PushID(label.c_str());

	bool valueChanged{ false };
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
	ImVec2 buttonSize{ lineHeight + 3.f, lineHeight };


	/** X value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values[0] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragFloat("##X", &values[0], 0.1f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);

	/** X value - End */
	/** Y value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values[1] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &values[1], 0.1f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);

	/** Y value - End */
	/** Z value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		values[2] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragFloat("##Z", &values[2], 0.1f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);
	/** Z value - End */

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return valueChanged;
}
bool InspectorUI::DrawXMINT2Controlls(const std::string& label, int* values, const float columnWidth, const int resetValue, const int min, const int max)
{
	ImGuiIO& io{ ImGui::GetIO() };
	ImFont* pBoldFont{ io.Fonts->Fonts[0] };

	ImGui::PushID(label.c_str());

	bool valueChanged{ false };
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
	ImVec2 buttonSize{ lineHeight + 3.f, lineHeight };


	/** X value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values[0] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragInt("##X", &values[0], 1.f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);

	/** X value - End */
	/** Y value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values[1] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragInt("##Y", &values[1], 1.f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);
	/** Y value - End */

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return valueChanged;
}
bool InspectorUI::DrawXMINT3Controlls(const std::string& label, int* values, const float columnWidth, const int resetValue, const int min, const int max)
{
	ImGuiIO& io{ ImGui::GetIO() };
	ImFont* pBoldFont{ io.Fonts->Fonts[0] };

	ImGui::PushID(label.c_str());

	bool valueChanged{ false };
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
	ImVec2 buttonSize{ lineHeight + 3.f, lineHeight };


	/** X value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("X", buttonSize))
	{
		values[0] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragInt("##X", &values[0], 1.f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);

	/** X value - End */
	/** Y value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("Y", buttonSize))
	{
		values[1] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragInt("##Y", &values[1], 1.f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopStyleColor(3);

	/** Y value - End */
	/** Z value - Begin */
	/** Buttoncolors */
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });

	ImGui::PushFont(pBoldFont);
	if (ImGui::Button("Z", buttonSize))
	{
		values[2] = resetValue;
		valueChanged = true;
	}
	ImGui::PopFont();

	ImGui::SameLine();
	if (ImGui::DragInt("##Z", &values[2], 1.f, min, max))
		valueChanged = true;
	ImGui::PopItemWidth();
	ImGui::PopStyleColor(3);
	/** Z value - End */

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return valueChanged;
}
