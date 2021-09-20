#pragma once
#include "Material.h"
#include <vector>

class Texture;
class Material_ProcGen final : public Material
{
	public:
		Material_ProcGen();
		virtual ~Material_ProcGen();

		Material_ProcGen(const Material_ProcGen&) = delete;
		Material_ProcGen& operator=(const Material_ProcGen&) = delete;
		Material_ProcGen(Material_ProcGen&&) = delete;
		Material_ProcGen& operator=(Material_ProcGen&&) = delete;

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