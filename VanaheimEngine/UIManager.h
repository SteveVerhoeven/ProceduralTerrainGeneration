#pragma once
#include <vector>

class UI;
class Window;
class Graphics;
struct ImGuiViewport;
class UIManager final
{
	public:
		UIManager(Window* pWindow);
		~UIManager();

		UIManager(const UIManager&) = delete;
		UIManager(UIManager&&) noexcept = delete;
		UIManager& operator=(const UIManager&) = delete;
		UIManager& operator=(UIManager&&) noexcept = delete;

		void Initialize();
		void PostInitialize();

		void BeginFrame();
		void Render();
		void EndFrame();

		void AddUI(UI* pUI) { m_pUIs.push_back(pUI); }
		template <class T>
		T* GetUI() const;

	private:
		Window* m_pWindow;
		ImGuiViewport* m_pMainViewport;
		std::vector<UI*> m_pUIs;

		void InitializeImGui(Window* pWindow, Graphics* pGraphics);
		void InitializeUIs();
		void ShutdownImGui();

		// Docking
		void OpenDockSpace();
		void CloseDockSpace();

		//Style
		void SetThemeColors();
};

template<class T>
inline T* UIManager::GetUI() const
{
	for (UI* pUI : m_pUIs)
	{
		T* pTUI = dynamic_cast<T*>(pUI);
		if (pTUI)
			return pTUI;
	}

	return nullptr;
}
