#pragma once
#include "../Libs/IMGUI/imgui.h"
#include <vector>

#include "Subject.h"

enum class UIButtonType
{
	ARROW_BUTTON,
	SLIDER_FLOAT,
	SLIDER_INT,
	CHECKBOX,
	LIST
};

class UI : public Subject
{		
	public:
		UI(const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size);
		~UI() = default;

		UI(const UI&) = delete;
		UI& operator=(const UI&) = delete;
		UI(UI&&) = delete;
		UI& operator=(UI&&) = delete;

		void ActivateUI() { m_RenderUI = true; }
		void DeactivateUI() { m_RenderUI = false; }

		virtual void ShowWindow() = 0;

	protected:
		bool m_RenderUI;
		std::string m_Name;
		DirectX::XMFLOAT2 m_Position;
		DirectX::XMFLOAT2 m_Size;

		void CreateWindowBase();
		void EndWindowBase();
		
	private:
};