#pragma once
#include <vector>

class GameObject;
class Material
{
	public:
		enum class SamplingType
		{
			POINT = 0,
			LINEAR = 1,
			ANISOTROPIC = 2
		};

		Material(const std::string& assetPath, const std::string& name);
		virtual ~Material();

		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
		Material(Material&&) = delete;
		Material& operator=(Material&&) = delete;

		virtual void Initialize(ID3D11Device* pDevice) = 0;
		virtual void Update(GameObject* pParentGameObject) = 0;

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::string& effectFilePath);

		ID3DX11Effect* GetEffect() const { return m_pDirectXEffect; }
		ID3DX11EffectTechnique* GetTechnique() const { return m_pDefaultTechnique; }

		const SamplingType& GetSamplingType() const { return m_SamplingState; }
		ID3DX11EffectSamplerVariable* GetSamplerVariable() const { return m_pSamplerVariable; }
		virtual ID3D11SamplerState* GetSamplerPoint() const { return m_pSamplerState_Point; }
		virtual ID3D11SamplerState* GetSamplerLinear() const { return m_pSamplerState_Linear; }
		virtual ID3D11SamplerState* GetSamplerAnisotropic() const { return m_pSamplerState_Anisotropic; }

		void SetName(const std::string& name) { m_Name = name; }
		std::string GetName() const { return m_Name; }

	protected:
		bool m_IsInitialized;
		std::string m_Name;
		std::string m_AssetPath;

		SamplingType m_SamplingState;
		ID3DX11EffectSamplerVariable* m_pSamplerVariable;
		ID3D11SamplerState* m_pSamplerState_Point;
		ID3D11SamplerState* m_pSamplerState_Linear;
		ID3D11SamplerState* m_pSamplerState_Anisotropic;

		ID3DX11Effect* m_pDirectXEffect;
		ID3DX11EffectTechnique* m_pDefaultTechnique;
		ID3DX11EffectMatrixVariable* m_pMat_WorldViewProjVariable;

		ID3DX11EffectTechnique* BindTechniqueVariable(const std::string& techniqueName);
		ID3DX11EffectMatrixVariable* BindMatrixVariable(const std::string& matrixName);
		ID3DX11EffectShaderResourceVariable* BindTextureVariable(const std::string& textureName);
		ID3DX11EffectRasterizerVariable* BindRasterizerStateVariable(const std::string& textureName);

	private:
		void CreateSamplerStates(ID3D11Device* pDevice, const D3D11_FILTER& filter);
};