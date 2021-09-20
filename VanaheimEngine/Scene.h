#pragma once

class Mesh;
class Material;
class Material_ProcGen;
class GameObject;
struct ProcGenSettings;
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
		GameObject* GetObjectByName(const std::string& name) const;
		std::vector<GameObject*> GetObjects() const { return m_pGameObjects; }

		void ActivateScene() { m_IsActive = true; }
		void DeactivateScene() { m_IsActive = false; }

	protected:
		void CreateCamera(const std::string& name, const DirectX::XMFLOAT3& position);
		void Create3DObject(const std::string& name, const DirectX::XMFLOAT3& position, const std::string& meshPath, Material* pMaterial);
		void Create3DObject(const std::string& name, const DirectX::XMFLOAT3& position, Mesh* pMesh, Material* pMaterial);
		void CreateUI();

	private:
		bool m_IsActive;
		std::string m_Name;
		std::vector<GameObject*> m_pGameObjects{};
};