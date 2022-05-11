#pragma once
#include <vector>

class Scene;
class Graphics;
class UIManager;
class SceneManager final
{
	public:
		SceneManager();
		~SceneManager();

		void Initialize();
		void PostInitialize();
		void Update(const float elapsedSec);
		void FixedUpdate(const float timeEachUpdate);
		void LateUpdate();
		void Render();

		void AddGameScene(Scene* pScene) { m_pGameScenes.push_back(pScene); }
		void AddMenuScene(Scene* pScene) { m_pMenuScenes.push_back(pScene); }
		void SetActiveGameSceneIndex(const int activeSceneIdx) { m_ActiveGameSceneIndex = activeSceneIdx; }
		void SetActiveMenuSceneIndex(const int activeSceneIdx) { m_ActiveGameSceneIndex = activeSceneIdx; }

		Scene* GetActiveGameSceneByIndex(const size_t sceneIndex) const { return m_pGameScenes[sceneIndex]; }
		Scene* GetActiveGameScene() const { return m_pGameScenes[m_ActiveGameSceneIndex]; }
		int GetActiveGameSceneIndex() const { return m_ActiveGameSceneIndex; }
		Scene* GetActiveMenuSceneByIndex(const size_t sceneIndex) const { return m_pMenuScenes[sceneIndex]; }
		Scene* GetActiveMenuScene() const { return m_pMenuScenes[m_ActiveMenuSceneIndex]; }
		int GetActiveMenuSceneIndex() const { return m_ActiveGameSceneIndex; }

		void DeactivateAllGameScenes();
		void DeactivateAllMenuScenes();

		void ActivateNextGameScene();

		void ActivateGameSceneByIndex(const size_t sceneIndex);
		void ActivateMenuSceneByIndex(const size_t sceneIndex);

		bool GetMenuIsActie() const { return m_MenuActive; }
		bool GetGameIsActie() const { return m_GameActive; }

	private:
		bool m_MenuActive;
		bool m_GameActive;
		int m_ActiveGameSceneIndex;
		int m_ActiveMenuSceneIndex;
		std::vector<Scene*> m_pGameScenes;
		std::vector<Scene*> m_pMenuScenes;
		Graphics* m_pGraphics;
		UIManager* m_pUIManager;
};