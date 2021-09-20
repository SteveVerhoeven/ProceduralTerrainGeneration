#pragma once
#include "Component.h"

class Mesh;
class Scene;
class Texture;
class Material;
class ModelComponent final : public Component
{
	public:
		ModelComponent(const std::string& path);
		ModelComponent(Mesh* pMesh);
		virtual ~ModelComponent() = default;

		ModelComponent(const ModelComponent&) = delete;
		ModelComponent(ModelComponent&&) noexcept = delete;
		ModelComponent& operator=(const ModelComponent&) = delete;
		ModelComponent& operator=(ModelComponent&&) noexcept = delete;

		void Render();
		
		void AddMaterial(Material* pMaterial);
		void AddTexture(Texture* pTexture) { m_pTextures.push_back(pTexture); }

		Mesh* GetMesh() const { return m_pMesh; }
		Material* GetMaterial() const { return m_pMaterial; }

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
		Mesh* m_pMesh;
		Material* m_pMaterial;
		std::string m_FilePath;
		std::vector<Texture*> m_pTextures;
};