#pragma once
#include "Material.h"
#include <vector>

class Texture;
class GameObject;
class Material_ProcGen_GPU final : public Material
{
	public:
		Material_ProcGen_GPU();
		virtual ~Material_ProcGen_GPU();

		Material_ProcGen_GPU(const Material_ProcGen_GPU&) = delete;
		Material_ProcGen_GPU& operator=(const Material_ProcGen_GPU&) = delete;
		Material_ProcGen_GPU(Material_ProcGen_GPU&&) = delete;
		Material_ProcGen_GPU& operator=(Material_ProcGen_GPU&&) = delete;

		void Initialize(ID3D11Device* pDevice) override;
		void Update(GameObject* pParentGameObject) override;

		void AddTexture(Texture* pTexture) { m_pTextures.push_back(pTexture); }

	protected:

	private:
		static ID3DX11EffectShaderResourceVariable* m_pNoiseSRVvariable;
		static ID3DX11EffectShaderResourceVariable* m_pColorSRVvariable;

		// Layout
		// [0] - Noise
		// [1] - Color
		std::vector<Texture*> m_pTextures;
};