#pragma once
#include <Game.h>

class VGame final : public Game
{
	public:
		VGame(const std::string& name);
		virtual ~VGame() = default;

		VGame(const VGame& other) = delete;
		VGame(VGame&& other) = delete;
		VGame& operator=(const VGame& other) = delete;
		VGame& operator=(VGame&& other) = delete;

		virtual void Initialize(HINSTANCE hInstance);
		virtual void PostInitialize();
	protected:

	private:
};