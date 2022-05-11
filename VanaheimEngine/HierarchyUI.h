#pragma once
#include "UI.h"

class Scene;
class InspectorUI;
class HierarchyUI final : public UI
{
	public:
		HierarchyUI();
		~HierarchyUI();

		HierarchyUI(const HierarchyUI&) = delete;
		HierarchyUI& operator=(const HierarchyUI&) = delete;
		HierarchyUI(HierarchyUI&&) = delete;
		HierarchyUI& operator=(HierarchyUI&&) = delete;

	protected:
		void Initialize() override;
		void ShowWindow() override;

	private:
		Scene* m_pScene;
		size_t m_SelectedGameObject;
		InspectorUI* m_pInspectorUI;

		void Draw();
};