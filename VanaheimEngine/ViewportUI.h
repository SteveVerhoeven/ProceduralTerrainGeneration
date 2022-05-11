#pragma once
#include "UI.h"

class ViewportUI final : public UI
{
	public:
		ViewportUI();
		~ViewportUI();

		ViewportUI(const ViewportUI&) = delete;
		ViewportUI& operator=(const ViewportUI&) = delete;
		ViewportUI(ViewportUI&&) = delete;
		ViewportUI& operator=(ViewportUI&&) = delete;

	protected:
		void Initialize() override;
		void ShowWindow() override;

	private:
};