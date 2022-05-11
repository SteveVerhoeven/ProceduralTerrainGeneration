#include "VanaheimPCH.h"
#include "Material.h"

#include "GameObject.h"

Material::Material(const std::string& assetPath, const std::string& name)
		 : m_IsInitialized(false)
		 , m_AssetPath(assetPath)
		 , m_Name(name)
		 , m_pDirectXEffect{ nullptr }
		 , m_pMat_WorldViewProjVariable{ nullptr }
		 , m_pDefaultTechnique{ nullptr }
		 , m_pSamplerVariable{ nullptr }
		 , m_pSamplerState_Point{ nullptr }
		 , m_pSamplerState_Linear{ nullptr }
		 , m_pSamplerState_Anisotropic{ nullptr }
		 , m_SamplingState{ SamplingType::POINT }
{}
Material::~Material()
{
	if (m_pMat_WorldViewProjVariable) DELETE_RESOURCE_VALID(m_pMat_WorldViewProjVariable);
	DELETE_RESOURCE(m_pSamplerState_Point);
	DELETE_RESOURCE(m_pSamplerState_Linear);
	DELETE_RESOURCE(m_pSamplerState_Anisotropic);
	if (m_pSamplerVariable) DELETE_RESOURCE_VALID(m_pSamplerVariable);
	if (m_pDefaultTechnique) DELETE_RESOURCE_VALID(m_pDefaultTechnique);
	if (m_pDirectXEffect) DELETE_RESOURCE_VALID(m_pDirectXEffect);
}

void Material::Initialize(ID3D11Device* pDevice)
{
	if (m_IsInitialized)
		return;

	// ********************************
	// Load effect
	// ********************************
	m_pDirectXEffect = LoadEffect(pDevice, m_AssetPath);
	if (!m_pDirectXEffect->IsValid())
	{
		LOG(ErrorLevel::LOG_FATAL, "Effect not loaded! \n File: Effect.cpp \n Line: 30;");
	}

	// ********************************
	// Bind sampler state
	// ********************************
	m_pSamplerVariable = m_pDirectXEffect->GetVariableByName("gSamplerState")->AsSampler();
	if (!m_pDirectXEffect->IsValid())
	{
		LOG(ErrorLevel::LOG_FATAL, "Sampler variable not found! \n File: Effect.cpp \n Line: 37;");
	}

	// ********************************
	// Bind matrix variables
	// ********************************
	m_pMat_WorldViewProjVariable = BindMatrixVariable("gWorldViewProj");

	// ********************************
	// Create Sampler States
	// ********************************
	CreateSamplerStates(pDevice, D3D11_FILTER_MIN_MAG_MIP_POINT);
	CreateSamplerStates(pDevice, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	CreateSamplerStates(pDevice, D3D11_FILTER_ANISOTROPIC);

	m_pDefaultTechnique = BindTechniqueVariable("DefaultTechnique");

	m_IsInitialized = true;
}
void Material::Update(GameObject* pParentGameObject)
{
	using namespace DirectX;

	CameraComponent* pCameraComponent{ Locator::GetSceneCameraService() };
	TransformComponent* pTransformComponent{ pParentGameObject->GetComponent<TransformComponent>() };

	DirectX::XMFLOAT4X4 world{ pTransformComponent->GetWorld() };
	DirectX::XMFLOAT4X4 view{ pCameraComponent->GetView() };
	DirectX::XMFLOAT4X4 projection{ pCameraComponent->GetProjection() };

	DirectX::XMMATRIX matrix_world{ DirectX::XMLoadFloat4x4(&world) };
	DirectX::XMMATRIX matrix_view{ DirectX::XMLoadFloat4x4(&view) };
	DirectX::XMMATRIX matrix_projection{ DirectX::XMLoadFloat4x4(&projection) };

	DirectX::XMMATRIX wvp{ matrix_world * matrix_view * matrix_projection };

	HRESULT hr{ m_pMat_WorldViewProjVariable->SetMatrix((float*)&wvp) };
	if (FAILED(hr))
		LOG_HRESULT(hr, "Material::Update", __FILE__, std::to_string(__LINE__));
}

// ------------------------------------
// Code from: DirectX project 
// During Graphics Programming 1 - DAE
// Author: Matthieu Delaere
// ------------------------------------
ID3DX11Effect* Material::LoadEffect(ID3D11Device* pDevice, const std::string& effectFilePath)
{
	HRESULT result = S_OK;
	ID3D10Blob* pErrorBlob{ nullptr };
	ID3DX11Effect* pEffect{ nullptr };

	DWORD shaderFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(std::wstring(effectFilePath.begin(), effectFilePath.end()).c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);
	if (FAILED(result))
	{
		// -------------
		// Log the error
		// -------------
		LOG_HRESULT(result, "Material::LoadEffect", __FILE__, std::to_string(__LINE__));
		if (pErrorBlob != nullptr)
		{
			char* pErrors = (char*)pErrorBlob->GetBufferPointer();
			std::wstringstream ss;
			for (unsigned int i{}; i < pErrorBlob->GetBufferSize(); ++i)
				ss << pErrors[i];

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << std::endl;
		}
		else
		{
			std::wstringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << std::wstring(effectFilePath.begin(), effectFilePath.end());
			std::wcout << ss.str() << std::endl;
			return nullptr;
		}
	}

	return pEffect;
}

ID3DX11EffectTechnique* Material::BindTechniqueVariable(const std::string& techniqueName)
{
	// ********************************
	// Bind techniques
	// ********************************
	ID3DX11EffectTechnique* pTechnique = m_pDirectXEffect->GetTechniqueByName(techniqueName.c_str());

	if (!pTechnique->IsValid())
	{
		std::string message{};
		message += "Technique not found!\n";
		message += "Name: " + techniqueName + '\n';
		message += "Line: 129";

		LOG(ErrorLevel::LOG_FATAL, message);
	}


	return pTechnique;
}
ID3DX11EffectMatrixVariable* Material::BindMatrixVariable(const std::string& matrixName)
{
	// ********************************
	// Bind matrix
	// ********************************
	ID3DX11EffectMatrixVariable* pMatrix = m_pDirectXEffect->GetVariableByName(matrixName.c_str())->AsMatrix();

	if (!pMatrix->IsValid())
	{
		std::string message{};
		message += "Matrix not found!\n";
		message += "Name: " + matrixName + '\n';
		message += "Line: 152";

		LOG(ErrorLevel::LOG_FATAL, message);
	}

	return pMatrix;
}
ID3DX11EffectShaderResourceVariable* Material::BindTextureVariable(const std::string& textureName)
{
	// ********************************
	// Bind texture
	// ********************************
	ID3DX11EffectShaderResourceVariable* pTexture = m_pDirectXEffect->GetVariableByName(textureName.c_str())->AsShaderResource();

	if (!pTexture->IsValid())
	{
		std::string message{};
		message += "Texture not found!\n";
		message += "Name: " + textureName + '\n';
		message += "Line: 175";

		LOG(ErrorLevel::LOG_FATAL, message);
	}

	return pTexture;
}
ID3DX11EffectRasterizerVariable* Material::BindRasterizerStateVariable(const std::string& rasterizerName)
{
	// ********************************
	// Bind rasterizer state
	// ********************************
	ID3DX11EffectRasterizerVariable* pRasterizerState = m_pDirectXEffect->GetVariableByName(rasterizerName.c_str())->AsRasterizer();

	if (!pRasterizerState->IsValid())
	{
		std::string message{};
		message += "Rasterizer state not found!\n";
		message += "Name: " + rasterizerName + '\n';
		message += "Line: 197";

		LOG(ErrorLevel::LOG_FATAL, message);
	}

	return pRasterizerState;
}

void Material::CreateSamplerStates(ID3D11Device* pDevice, const D3D11_FILTER& filter)
{
	D3D11_SAMPLER_DESC samplerDescPoint;
	samplerDescPoint.Filter = filter;
	samplerDescPoint.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescPoint.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDescPoint.MipLODBias = 0;
	samplerDescPoint.MaxAnisotropy = 0;

	const D3D11_SAMPLER_DESC* pSamplerStateDesc{ &samplerDescPoint };
	ID3D11SamplerState** ppSamplerState{};

	if (filter == D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT)
		ppSamplerState = &m_pSamplerState_Point;
	else if (filter == D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR)
		ppSamplerState = &m_pSamplerState_Linear;
	else if (filter == D3D11_FILTER::D3D11_FILTER_ANISOTROPIC)
		ppSamplerState = &m_pSamplerState_Anisotropic;

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createsamplerstate
	HRESULT hr{ pDevice->CreateSamplerState(pSamplerStateDesc, ppSamplerState) };
	LOG_HRESULT(hr, "Material::CreateSamplerStates", std::string(__FILE__), std::to_string(__LINE__));
}