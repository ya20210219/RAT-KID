#pragma once
class imguiManager
{
private:
	// Data
	static ID3D11Device* m_pd3dDevice;
	static ID3D11DeviceContext* m_pd3dDeviceContext;
	static IDXGISwapChain* m_pSwapChain;
	static ID3D11RenderTargetView* m_mainRenderTargetView;

public:

	static void Init();
	static void Update();
	static void Uninit();
	static void Draw();
};

