#pragma once
#include "UI.h"
#include "UIData.h"

class InspectorUI final : public UI
{
	public:
		InspectorUI();
		~InspectorUI();

		InspectorUI(const InspectorUI&) = delete;
		InspectorUI& operator=(const InspectorUI&) = delete;
		InspectorUI(InspectorUI&&) = delete;
		InspectorUI& operator=(InspectorUI&&) = delete;

		void StoreVariable(InspectorVariable* inspectorVar) { m_Variables.push_back(inspectorVar); }

	protected:
		void ShowWindow() override;
	
	private:
		void SetPositionAndSize();
		std::vector<InspectorVariable*> m_Variables;

		void DisplayInspectorVariables();
};
