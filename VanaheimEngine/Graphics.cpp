#include "VanaheimPCH.h"
#include "Graphics.h"
#include "Window.h"

#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

Graphics::Graphics(HWND hWnd, const int width, const int height)
		 : m_Window(hWnd)
		 , m_pDevice(nullptr)
		 , m_pDeviceContext(nullptr)
		 , m_pDXGIFactory(nullptr)
		 , m_pSwapChain(nullptr)
		 , m_pDepthStencilBuffer(nullptr)
		 , m_pDepthStencilView(nullptr)
		 , m_pRenderTargetBuffer_Main(nullptr)
		 , m_pRenderTargetView_Main(nullptr)
		 , m_pRenderTargetBuffer_Game(nullptr)
		 , m_pRenderTargetView_Game(nullptr)
		 , m_pShaderResourceView_Game(nullptr)
{
	HRESULT hr{ InitializeDirectX(width, height) };
	if (hr != S_OK)
		LOG_HRESULT(hr, "Graphics::InitializeDirectX", __FILE__, std::to_string(__LINE__));
}
Graphics::Graphics(Window* pWindow)
		 : Graphics(pWindow->GetWindowHandle(), pWindow->GetWindowWidth(), pWindow->GetWindowHeight())
{}
Graphics::~Graphics()
{
	DELETE_RESOURCE(m_pRenderTargetView_Main);
	DELETE_RESOURCE(m_pRenderTargetView_Game);
	DELETE_RESOURCE(m_pRenderTargetBuffer_Main);
	DELETE_RESOURCE(m_pRenderTargetBuffer_Game);
	DELETE_RESOURCE(m_pShaderResourceView_Game);
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
//void Graphics::SetWindowDimensions(const UINT& x, const UINT& y)
//{
//	m_Width = x;
//	m_Height = y;
//}
//void Graphics::SetFullScreen(const bool /*fullScreenOn*/)
//{
//	BOOL currentState{};
//	m_pSwapChain->GetFullscreenState(&currentState, NULL);
//
//	bool newState = false;
//	if (currentState == false)
//		newState = true;
//
//	m_pSwapChain->SetFullscreenState(newState, NULL);
//}
//
//void Graphics::ResizeWindow(const DirectX::XMINT2& /*dimensions*/)
//{
//	if (m_pSwapChain)
//	{
//		//m_pDeviceContext->OMSetRenderTargets(0, 0, 0);
//
//		//// Release all outstanding references to the swap chain's buffers.
//		//m_pRenderTargetBuffer_Main->Release();
//		//m_pRenderTargetBuffer_Game->Release();
//
//		//HRESULT hr;
//		//// Preserve the existing buffer count and format.
//		//// Automatically choose the width and height to match the client rect for HWNDs.
//		//hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
//
//		//// Perform error handling here!
//
//		//// Get buffer and create a render-target-view.
//		//ID3D11Texture2D* pBuffer;
//		//hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
//		//	(void**)&pBuffer);
//		//// Perform error handling here!
//
//		//hr = m_pDevice->CreateRenderTargetView(pBuffer, NULL, &m_pRenderTargetBuffer_Main);
//		//// Perform error handling here!
//		//pBuffer->Release();
//
//		//g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
//
//		//// Set up the viewport.
//		//D3D11_VIEWPORT vp;
//		//vp.Width = width;
//		//vp.Height = height;
//		//vp.MinDepth = 0.0f;
//		//vp.MaxDepth = 1.0f;
//		//vp.TopLeftX = 0;
//		//vp.TopLeftY = 0;
//		//g_pd3dDeviceContext->RSSetViewports(1, &vp);
//	}
//}

HRESULT Graphics::InitializeDirectX(const int width, const int height)
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
	hr = CreateSwapChain(width, height);
	if (FAILED(hr))
		return hr;

	//m_pSwapChain->SetFullscreenState(true, NULL);

	// *************************************************************
	// Create Depth/Stencil Buffer and View
	// *************************************************************
	hr = CreateDepth_Stencil_Resources(width, height);
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Create Render target Buffer - Main window
	// *************************************************************
	hr = CreateRenderTarget_Main();
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Create Render target Buffer - Main window
	// *************************************************************
	hr = CreateRenderTarget_Game(width, height);
	if (FAILED(hr))
		return hr;

	//// *************************************************************
	//// Create Depth/Stencil Buffer and View
	//// *************************************************************
	hr = CreateShaderResourceView();
	if (FAILED(hr))
		return hr;

	// *************************************************************
	// Bind the views to the output merger stage
	// *************************************************************
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView_Main, m_pDepthStencilView);
	
	// *************************************************************
	// Set Viewport
	// *************************************************************
	D3D11_VIEWPORT viewPort{};
	viewPort.Width = static_cast<float>(width);
	viewPort.Height = static_cast<float>(height);
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

	// Creating device
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
HRESULT Graphics::CreateSwapChain(const int width, const int height)
{
	// Create SwapChain descriptor
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
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
HRESULT Graphics::CreateDepth_Stencil_Resources(const int width, const int height)
{
	HRESULT hr{};

	// Create SwapChain descriptor
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
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
	/* CreateTexture2D - Parameters */
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
	return m_pDevice->CreateDepthStencilView(pDepthStencilBuffer, pDepthStencilViewDesc, ppDepthStencilView);
}
HRESULT Graphics::CreateRenderTarget_Main()
{
	HRESULT hr{};

	// Create RenderTargetBuffer
	/* GetBuffer - Parameters */
	UINT buffer{ 0 };
	REFIID riid2{ __uuidof(ID3D11Texture2D) };
	void** ppSurface{ reinterpret_cast<void**>(&m_pRenderTargetBuffer_Main) };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
	hr = m_pSwapChain->GetBuffer(buffer, riid2, ppSurface);
	if (FAILED(hr))
		return hr;

	// Create RendertargetView
	/* CreateRenderTargetView - Parameters */
	ID3D11Resource* pRenderTargetBuffer{ m_pRenderTargetBuffer_Main };
	const D3D11_RENDER_TARGET_VIEW_DESC* pRenderTargetViewDesc{ NULL };
	ID3D11RenderTargetView** ppRTView{ &m_pRenderTargetView_Main };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createrendertargetview
	return m_pDevice->CreateRenderTargetView(pRenderTargetBuffer, pRenderTargetViewDesc, ppRTView);
}
HRESULT Graphics::CreateRenderTarget_Game(const int width, const int height)
{
	HRESULT hr{};

	// Create texture descriptor
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create Shader resource view
	/* CreateShaderResourceView - Parameters */
	const D3D11_TEXTURE2D_DESC* pDesc{ &textureDesc };
	const D3D11_SUBRESOURCE_DATA* pInitialData{ nullptr };
	ID3D11Texture2D** ppTexture2D{ &m_pRenderTargetBuffer_Game };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createtexture2d
	hr = m_pDevice->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
	if (FAILED(hr))
		return hr;

	// Create render target view
	/* CreateRenderTargetView - Parameters */
	ID3D11Resource* pResource{ m_pRenderTargetBuffer_Game };
	const D3D11_RENDER_TARGET_VIEW_DESC* pDescRTV{ nullptr };
	ID3D11RenderTargetView** ppRTView{ &m_pRenderTargetView_Game };
	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createrendertargetview
	return m_pDevice->CreateRenderTargetView(pResource, pDescRTV, ppRTView);
}
HRESULT Graphics::CreateShaderResourceView()
{
	// Create Shader resource view
	/* CreateShaderResourceView - Parameters */
	ID3D11Resource* pResource{ m_pRenderTargetBuffer_Game };
	const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc{ nullptr };
	ID3D11ShaderResourceView** ppSRView{ &m_pShaderResourceView_Game };

	// Explanation for all parameters in link below
	// Reference: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-createshaderresourceview
	return m_pDevice->CreateShaderResourceView(pResource, pDesc, ppSRView);
}

void Graphics::ClearBackbuffer()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView_Main, reinterpret_cast<const float*>(&DirectX::Colors::Black));
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView_Game, reinterpret_cast<const float*>(&DirectX::Colors::Black));
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Graphics::PresentBackbuffer()
{
	m_pSwapChain->Present(0, 0);
}

void Graphics::SetMainRenderTarget()
{
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView_Main, m_pDepthStencilView);
}
void Graphics::SetGameRenderTarget()
{
	m_pDeviceContext->PSSetShaderResources(0, 0, &m_pShaderResourceView_Game);
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView_Game, m_pDepthStencilView);
}