#pragma once
class Timer;
class Window;
class Graphics;
class UIManager;
class DebugLogger;
class SceneManager;
class InputManager;
class ResourceManager;
class GeneratorManager;
class VanaheimEngine final
{
	public:
		VanaheimEngine();
		~VanaheimEngine();

		VanaheimEngine(const VanaheimEngine&) = delete;
		VanaheimEngine& operator=(const VanaheimEngine&) = delete;
		VanaheimEngine(VanaheimEngine&&) = delete;
		VanaheimEngine& operator=(VanaheimEngine&&) = delete;

		void Initialize(HINSTANCE instance);
		void Update();

	protected:
	private:
		Timer* m_pTimer;
		Window* m_pWindow;
		Graphics* m_pGraphics;
		UIManager* m_pUIManager;
		DebugLogger* m_pDebugLogger;
		SceneManager* m_pSceneManager;
		InputManager* m_pInputManager;
		ResourceManager* m_pResourceManager;
		GeneratorManager* m_pGeneratorManager;

		void InitializeLocator();

		void Render();
};