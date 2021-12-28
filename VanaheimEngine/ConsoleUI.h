#pragma once
#include "UI.h"

class ConsoleUI final : public UI
{
	public:
		ConsoleUI();
		~ConsoleUI() = default;

		ConsoleUI(const ConsoleUI&) = delete;
		ConsoleUI& operator=(const ConsoleUI&) = delete;
		ConsoleUI(ConsoleUI&&) = delete;
		ConsoleUI& operator=(ConsoleUI&&) = delete;

		void Log(const std::string& consoleMessage) { m_ConsoleMessages.push_back(consoleMessage); }

	protected:
		void ShowWindow() override;

	private:
		void SetPositionAndSize();

		std::vector<std::string> m_ConsoleMessages;
};
