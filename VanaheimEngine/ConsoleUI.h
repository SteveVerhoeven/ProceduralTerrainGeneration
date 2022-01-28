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

		void Log(const std::string& consoleMessage) 
		{ 
			//if (m_ConsoleMessages.size() >= 25)
			//	m_ConsoleMessages.clear();
			//m_ConsoleMessages.push_back(consoleMessage); 

			m_ConsoleMessages[m_CurrentConsoleSize++] = consoleMessage;
			if (m_CurrentConsoleSize >= m_MaxConsoleSize)
				m_CurrentConsoleSize = 0;
		}

	protected:
		void ShowWindow() override;

	private:
		static const int m_MaxConsoleSize = 25;
		int m_CurrentConsoleSize;
		//std::vector<std::string> m_ConsoleMessages;
		std::string m_ConsoleMessages[m_MaxConsoleSize] ;

		void SetPositionAndSize();


};
