#pragma once

class UI;
class UIManager final
{
	public:
		UIManager(Window* pWindow, Graphics* pGraphics);
		~UIManager();

		UIManager(const UIManager&) = delete;
		UIManager(UIManager&&) noexcept = delete;
		UIManager& operator=(const UIManager&) = delete;
		UIManager& operator=(UIManager&&) noexcept = delete;

		void BeginFrame();
		void EndFrame();

		void AddUI(UI* pUI) { m_pUIs.push_back(pUI); }
		template <class T>
		T* GetUI() const;

	private:
		std::vector<UI*> m_pUIs;

		void InitializeImGui(Window* pWindow, Graphics* pGraphics);
		void ShutdownImGui();
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
