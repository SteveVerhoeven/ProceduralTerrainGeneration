#pragma once

class Graphics final
{
	public:
		Graphics(HWND hWnd, const int width, const int height);
		~Graphics();

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		Graphics(Graphics&&) = delete;
		Graphics& operator=(Graphics&&) = delete;

		void ClearBackbuffer();
		void PresentBackbuffer();

		ID3D11Device* GetDevice() const { return m_pDevice; }
		ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }

	protected:
	private:
		HWND m_Window;
		UINT m_Width;
		UINT m_Height;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		IDXGIFactory* m_pDXGIFactory;
		IDXGISwapChain* m_pSwapChain;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11Resource* m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;

		HRESULT InitializeDirectX();
		HRESULT CreateDevice_Context();
		HRESULT CreateFactory();
		HRESULT CreateSwapChain();
		HRESULT CreateDepth_Stencil_Resources();
};