#include "pch.h"
#include "Material_ProcGen.h"

#include "Texture.h"

ID3DX11EffectShaderResourceVariable* Material_ProcGen::m_pNoiseSRVvariable{ nullptr };
ID3DX11EffectShaderResourceVariable* Material_ProcGen::m_pColorSRVvariable{ nullptr };

Material_ProcGen::Material_ProcGen()
				 : Material("./Resources/Shaders/PosCol3D_ProceduralGenerator.fx", "ProceduralGenerator")
				 , m_pTextures(std::vector<Texture*>())
{}
Material_ProcGen::~Material_ProcGen()
{
	DELETE_RESOURCE(m_pNoiseSRVvariable);
	DELETE_RESOURCE(m_pColorSRVvariable);

	m_pTextures.clear();
}

void Material_ProcGen::Initialize(ID3D11Device* pDevice)
{
	Material::Initialize(pDevice);

	if (!m_pNoiseSRVvariable)
		m_pNoiseSRVvariable = BindTextureVariable("gNoiseMap");
	if (!m_pColorSRVvariable)
		m_pColorSRVvariable = BindTextureVariable("gColorMap");
}

void Material_ProcGen::Update(GameObject* pParentGameObject)
{
	Material::Update(pParentGameObject);
	
	DebugLogger* pDebugLogger{ Locator::GetDebugLoggerService() };
	if (m_pTextures[0] && m_pNoiseSRVvariable)
		pDebugLogger->LogHRESULT(m_pNoiseSRVvariable->SetResource(m_pTextures[0]->GetShaderResourceView()), "Material_ProcGen::Update", __FILE__, std::to_string(__LINE__));

	if (m_pTextures[1] && m_pColorSRVvariable)
		pDebugLogger->LogHRESULT(m_pColorSRVvariable->SetResource(m_pTextures[1]->GetShaderResourceView()), "Material_ProcGen::Update", __FILE__, std::to_string(__LINE__));
}