#include "pch.h"
#include "Graphics.h"

#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

Graphics::Graphics(HWND hWnd, const int width, const int height)
	: m_Window(hWnd)
	, m_Width((UINT)width)
	, m_Height((UINT)height)
{
	HRESULT hr{ InitializeDirectX() };
	if (hr != S_OK)
		Locator::GetDebugLoggerService()->LogHRESULT(hr, "Graphics::InitializeDirectX", __FILE__, std::to_string(__LINE__));
}
Graphics::~Graphics()
{
	DELETE_RESOURCE(m_pRenderTargetView);
	DELETE_RESOURCE(m_pRenderTargetBuffer);
	DELETE_RESOURCE(m_pDepthStencilView);
	DELETE_RESOURCE(m_pDepthStencilBuffer);
	DELETE_RESOURCE(m_pSwapChain);
	DELETE_RESOURCE(m_pDevice);
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
		m_pDeviceContext->Flush();
		m_pDeviceContext->Release();
	}
	DELETE_RESOURCE(m_pDXGIFactory);
}

HRESULT Graphics::InitializeDirectX()
{
	HRESULT hr{};

	// *************************************************************
	// Create Device and Device context, using hardware acceleration
	// *************************************************************
	hr = CreateDevice_Context();
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Create DXGI Factory to create the SwapChain based on hardware
	// *************************************************************
	hr = CreateFactory();
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Create SwapChain
	// *************************************************************
	hr = CreateSwapChain();
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Create Depth/Stencil Buffer and View
	// *************************************************************
	hr = CreateDepth_Stencil_Resources();
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Bind the views to the output merger stage
	// *************************************************************
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// *************************************************************
	// Set Viewport
	// *************************************************************
	D3D11_VIEWPORT viewPort{};
	viewPort.Width = static_cast<float>(m_Width);
	viewPort.Height = static_cast<float>(m_Height);
	viewPort.TopLeftX = 0.f;
	viewPort.TopLeftY = 0.f;
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;
	m_pDeviceContext->RSSetViewports(1, &viewPort);

	return hr;
}
HRESULT Graphics::CreateDevice_Context()
{
	uint32_t createDeviceFlags{ 0 };
	D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
	#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
	#endif
	/* D3D11CreateDevice - Parameters */
	IDXGIAdapter* pAdapter{ 0 };
	D3D_DRIVER_TYPE driverType{ D3D_DRIVER_TYPE_HARDWARE };
	HMODULE software{ 0 };
	UINT flags{ createDeviceFlags };
	const D3D_FEATURE_LEVEL* pFeatureLevels{ 0 };
	UINT featureLevels{ 0 };
	UINT sdkVersion{ D3D11_SDK_VERSION };
	ID3D11Device** ppDevice{ &m_pDevice };
	D3D_FEATURE_LEVEL* pFeatureLevel{ &featureLevel };
	ID3D11DeviceContext** ppImmediateContext{ &m_pDeviceContext };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	return D3D11CreateDevice(pAdapter, driverType, software, flags, pFeatureLevels, featureLevels, sdkVersion, ppDevice, pFeatureLevel, ppImmediateContext);
}
HRESULT Graphics::CreateFactory()
{
	/* CreateDXGIFactory - Parameters */
	REFIID riid1{ __uuidof(IDXGIFactory) };
	void** ppFactory{ reinterpret_cast<void**>(&m_pDXGIFactory) };		// const??

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-createdxgifactory
	return CreateDXGIFactory(riid1, ppFactory);
}
HRESULT Graphics::CreateSwapChain()
{
	// Create SwapChain descriptor
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = m_Width;
	swapChainDesc.BufferDesc.Height = m_Height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// UNORM = unsigned normalized integer
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = m_Window;

	// Create SwapChain and hook it into the handle of the SDL window
	/* CreateDXGIFactory - Parameters */
	IUnknown* pDevice{ m_pDevice };
	DXGI_SWAP_CHAIN_DESC* pSwapChainDesc{ &swapChainDesc };
	IDXGISwapChain** ppSwapChain{ &m_pSwapChain };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgifactory-createswapchain
	return m_pDXGIFactory->CreateSwapChain(pDevice, pSwapChainDesc, ppSwapChain);
}
HRESULT Graphics::CreateDepth_Stencil_Resources()
{
	HRESULT hr{};

	// Create SwapChain descriptor
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Describe resource view for depth/stencil buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create Texture2D
	/* CreateDXGIFactory - Parameters */
	const D3D11_TEXTURE2D_DESC* pDepthStencilDesc{ &depthStencilDesc };
	const D3D11_SUBRESOURCE_DATA* pInitialData{ NULL };
	ID3D11Texture2D** ppTexture2D{ &m_pDepthStencilBuffer };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createtexture2d
	hr = m_pDevice->CreateTexture2D(pDepthStencilDesc, pInitialData, ppTexture2D);
	if (FAILED(hr))
		return hr;

	// Create DepthStencilView
	/* CreateDXGIFactory - Parameters */
	ID3D11Resource* pDepthStencilBuffer{ m_pDepthStencilBuffer };
	const D3D11_DEPTH_STENCIL_VIEW_DESC* pDepthStencilViewDesc{ &depthStencilViewDesc };
	ID3D11DepthStencilView** ppDepthStencilView{ &m_pDepthStencilView };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createdepthstencilview
	hr = m_pDevice->CreateDepthStencilView(pDepthStencilBuffer, pDepthStencilViewDesc, ppDepthStencilView);
	if (FAILED(hr))
		return hr;

	// Create RenderTargetBuffer
	/* CreateDXGIFactory - Parameters */
	UINT buffer{ 0 };
	REFIID riid2{ __uuidof(ID3D11Texture2D) };
	void** ppSurface{ reinterpret_cast<void**>(&m_pRenderTargetBuffer) };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
	hr = m_pSwapChain->GetBuffer(buffer, riid2, ppSurface);
	if (FAILED(hr))
		return hr;

	// Create RendertargetView
	/* CreateDXGIFactory - Parameters */
	ID3D11Resource* pRenderTargetBuffer{ m_pRenderTargetBuffer };
	const D3D11_RENDER_TARGET_VIEW_DESC* pRenderTargetViewDesc{ NULL };
	ID3D11RenderTargetView** ppRTView{ &m_pRenderTargetView };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createrendertargetview
	return m_pDevice->CreateRenderTargetView(pRenderTargetBuffer, pRenderTargetViewDesc, ppRTView);
}

void Graphics::ClearBackbuffer()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::CornflowerBlue));
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Graphics::PresentBackbuffer()
{
	m_pSwapChain->Present(0, 0);
}