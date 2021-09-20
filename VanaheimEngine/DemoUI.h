#pragma once
#include "UI.h"

class DemoUI final : public UI
{
	public:
		DemoUI(const bool showDemo, const std::string& windowName, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size);
		~DemoUI() = default;

		DemoUI(const DemoUI&) = delete;
		DemoUI& operator=(const DemoUI&) = delete;
		DemoUI(DemoUI&&) = delete;
		DemoUI& operator=(DemoUI&&) = delete;

		virtual void ShowWindow() override;

	protected:
	private:
		bool m_ShowDemo;
};