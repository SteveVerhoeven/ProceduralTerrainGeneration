#include "VanaheimPCH.h"
#include "Texture.h"

#include "Window.h"
#include "Graphics.h"

#include <algorithm>

Texture::Texture(const std::string& filePath)
		: m_pShaderResourceView(nullptr)
		, m_Path(filePath)
{
	Initialize(filePath);
}
Texture::~Texture()
{
	DELETE_RESOURCE(m_pShaderResourceView);
}

void Texture::Initialize(const std::string& filePath)
{
	// Reference: https://cpp.hotexamples.com/examples/-/-/LoadFromWICFile/cpp-loadfromwicfile-function-examples.html
	ID3D11Device* pDevice{ Locator::GetGraphicsService()->GetDevice() };

	// ********************
	// Load Image from File
	// ********************
	DirectX::TexMetadata info{};
	DirectX::ScratchImage* pImage{ new DirectX::ScratchImage() };
	LoadImageFromFile(pImage, info, filePath);

	// *****************************
	// Create a Shader Resource View
	// *****************************
	/* CreateShaderResourceView - parameters */
	ID3D11Device* pD3DDevice{ pDevice };
	const DirectX::Image* srcImages{ pImage->GetImages() };
	size_t nimages{ pImage->GetImageCount() };
	const DirectX::TexMetadata& metadata{ info };
	ID3D11ShaderResourceView** ppSRV{ &m_pShaderResourceView };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createshaderresourceview
	LOG_HRESULT(DirectX::CreateShaderResourceView(pD3DDevice, srcImages, nimages, metadata, ppSRV), "Texture::Initialize", __FILE__, std::to_string(__LINE__));

	DELETE_POINTER(pImage);
}

void Texture::onNotify(ObserverEvent event)
{
	if (event == ObserverEvent::REBUILD_LANDSCAPE)
	{
		DELETE_RESOURCE(m_pShaderResourceView);
		Initialize(m_Path);
	}
}

void Texture::LoadImageFromFile(DirectX::ScratchImage* pImage, DirectX::TexMetadata& info, const std::string& filePath)
{
	// Reference: https://cpp.hotexamples.com/examples/-/-/LoadFromWICFile/cpp-loadfromwicfile-function-examples.html
	std::string extension = filePath.substr(filePath.size() - 4, 4);
	std::wstring widePath = std::wstring(filePath.begin(), filePath.end());

	if (extension.find(".dds") != std::string::npos)
	{
		LOG_HRESULT(LoadFromDDSFile(widePath.c_str(), DirectX::DDS_FLAGS_NONE, &info, *pImage), "Texture::LoadImageFromFile", __FILE__, std::to_string(__LINE__));
	}
	else if (extension.find(".tga") != std::string::npos)
	{
		LOG_HRESULT(LoadFromTGAFile(widePath.c_str(), &info, *pImage), "Texture::LoadImageFromFile", __FILE__, std::to_string(__LINE__));
	}
	else
	{
		LOG_HRESULT(LoadFromWICFile(widePath.c_str(), DirectX::TEX_FILTER_DEFAULT, &info, *pImage), "Texture::LoadImageFromFile", __FILE__, std::to_string(__LINE__));
	}
}