#pragma once

class Graphics final
{
	public:
		Graphics(HWND hWnd, const int width, const int height);
		Graphics(Window* pWindow);
		~Graphics();

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		Graphics(Graphics&&) = delete;
		Graphics& operator=(Graphics&&) = delete;

		void ClearBackbuffer();
		void PresentBackbuffer();

		inline ID3D11Device* GetDevice() const { return m_pDevice; }
		inline ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }
		inline IDXGISwapChain* GetSwapChain() const { return m_pSwapChain; }
		inline ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pShaderResourceView_Game; }
		
		void SetMainRenderTarget();
		void SetGameRenderTarget();

		//void SetWindowDimensions(const UINT& x, const UINT& y);
		//void SetFullScreen(const bool fullScreenOn);
		//
		//void ResizeWindow(const DirectX::XMINT2& dimensions);

	protected:
	private:
		HWND m_Window;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		IDXGIFactory* m_pDXGIFactory;
		IDXGISwapChain* m_pSwapChain;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11Resource* m_pRenderTargetBuffer_Main;
		ID3D11RenderTargetView* m_pRenderTargetView_Main;
		
		ID3D11Texture2D* m_pRenderTargetBuffer_Game;
		ID3D11RenderTargetView* m_pRenderTargetView_Game;
		ID3D11ShaderResourceView* m_pShaderResourceView_Game;

		HRESULT InitializeDirectX(const int width, const int height);
		HRESULT CreateDevice_Context();
		HRESULT CreateFactory();
		HRESULT CreateSwapChain(const int width, const int height);
		HRESULT CreateDepth_Stencil_Resources(const int width, const int height);
		HRESULT CreateRenderTarget_Main();
		HRESULT CreateRenderTarget_Game(const int width, const int height);
		HRESULT CreateShaderResourceView();
};