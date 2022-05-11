#pragma once
#include "Component.h"
#include <string>
#include <vector>

class Line;
class Scene;
class Texture;
class Material;
class LineComponent final : public Component
{
	public:
		LineComponent(Line* pLine);
		virtual ~LineComponent();

		LineComponent(const LineComponent&) = delete;
		LineComponent(LineComponent&&) noexcept = delete;
		LineComponent& operator=(const LineComponent&) = delete;
		LineComponent& operator=(LineComponent&&) noexcept = delete;
	
		void Render();

		void AddMaterial(Material* pMaterial);
		void AddTexture(Texture* pTexture) { m_pTextures.push_back(pTexture); }

		Line* GetMesh() const { return m_pLine; }
		Material* GetMaterial() const { return m_pMaterial; }

		// Serialization
		//void Serialize(YAML::Emitter& out) override;

	protected:
		virtual void Initialize(Scene* pParentScene) override;
		virtual void PostInitialize(Scene* pParentScene) override;
		virtual void Update(const float elapsedSec) override;
		virtual void FixedUpdate(const float timeEachUpdate) override;

	private:
		Line* m_pLine;
		Material* m_pMaterial;
		std::string m_FilePath;
		std::vector<Texture*> m_pTextures;
};