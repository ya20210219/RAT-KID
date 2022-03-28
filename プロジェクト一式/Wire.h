#pragma once

#include "GameObject.h"

#define WIRE_WIDTH (0.1f)

class Wire : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	float m_WireWidth;

	D3DXVECTOR3 m_WireVec;

public:

	void Init();
	void UnInit();
	void Update();
	void Draw();

	void UpdateWireVertexPos(D3DXVECTOR3 pPos, D3DXVECTOR3 wPos, D3DXVECTOR3 cPos);

	void SetHPGaugeSize(float num)
	{
		m_WireWidth = num;
	}
};
