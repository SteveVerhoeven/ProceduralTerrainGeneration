#pragma once
#include <Game.h>

class Scene_Dev;
class Scene_ProcGen;
class DevGame final : public Game
{
	public:
		DevGame(const std::string& name);
		virtual ~DevGame() = default;

		DevGame(const DevGame& other) = delete;
		DevGame(DevGame&& other) = delete;
		DevGame& operator=(const DevGame& other) = delete;
		DevGame& operator=(DevGame&& other) = delete;

		virtual void Initialize(HINSTANCE hInstance);
		virtual void PostInitialize();
	protected:

	private:
		Scene_Dev* m_pScene_Dev;
		Scene_ProcGen* m_pScene_ProcGen;
};