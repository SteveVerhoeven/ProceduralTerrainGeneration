#pragma once

class Mesh;
class Line;
class Material;
class Material_ProcGen;
class GameObject;
class Scene
{
	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		virtual void Initialize();
		virtual void PostInitialize();
		virtual void Update(const float elapsedSec);
		virtual void FixedUpdate(const float timeEachUpdate);
		virtual void LateUpdate();
		virtual void Render() const;

		void AddGameObject(GameObject* pObject);
		void RemoveGameObject(GameObject* pObject);
		GameObject* GetObjectByName(const std::string& name) const;
		const std::vector<GameObject*>& GetObjects() const { return m_pGameObjects; }

		void ActivateScene() { m_IsActive = true; }
		void DeactivateScene() { m_IsActive = false; }
		
		void CreateLineObject(const std::string& name, const DirectX::XMFLOAT3& possition, Line* pLine);

		void SetSceneCamera(GameObject* pCamera) { m_pSceneCameraGO = pCamera; }
		GameObject* GetSceneCamera() const { return m_pSceneCameraGO; }

		void SetCleanSeneFlag() { m_Cleanup = true; }

	protected:
		void CreateCamera(const std::string& name, const DirectX::XMFLOAT3& position, const bool isMainCamera);
		void CreateSceneCamera(const std::string& name, const DirectX::XMFLOAT3& position);
		void Create3DObject(const std::string& name, const DirectX::XMFLOAT3& position, const std::string& meshPath, Material* pMaterial);
		void Create3DObject(const std::string& name, const DirectX::XMFLOAT3& possition, Mesh* pMesh, Material* pMaterial);

		/** Function that creates the inputs for the scene camera */
		void CreateBaseInputs();

	private:
		bool m_Cleanup;
		bool m_IsActive;
		std::string m_Name;
		GameObject* m_pSceneCameraGO;
		std::vector<GameObject*> m_pGameObjects{};

		void CreateSceneCamera();
		void CleanScene();
};