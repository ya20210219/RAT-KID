#pragma once

#include "GameObject.h"

#define COLOR_RED (D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f))
#define COLOR_GREEN (D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f))
#define COLOR_BLUE (D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f))
#define COLOR_YELLOW (D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f))

class DebugForwardRay : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	float m_HPGaugeSize;
public:

	void Init();
	void UnInit();
	void Update();
	void Draw();

	void SetHPGaugeSize(float num)
	{
		m_HPGaugeSize = num;
	}
};