#pragma once
#include <string>

class VanaheimEngine;
class Game
{
	public:
		Game(const std::string& name);
		virtual ~Game();

		Game(const Game& other) = delete;
		Game(Game&& other) = delete;
		Game& operator=(const Game& other) = delete;
		Game& operator=(Game&& other) = delete;
		
		int GameLoop();

	protected:
		virtual void Initialize(HINSTANCE hInstance) = 0;
		virtual void PostInitialize() = 0;

		VanaheimEngine* GetEngine() { return m_pEngine; }

	private:
		std::string m_Name;
		VanaheimEngine* m_pEngine;
};