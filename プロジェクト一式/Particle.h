#pragma once

#include "GameObject.h"

#define PARTICLE_MAX		100					// パーティクルの数

class Particle : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	D3DXVECTOR2 Vel;
	D3DXVECTOR2 Ace;
	bool		m_isUse;

public:

	void Init();
	void UnInit();
	void Update();
	void Draw();

	void SetVel(float x, float y)
	{
		Vel.x = x;
		Vel.y = y;

		m_Position.x = SCREEN_WIDTH / 2;
		m_Position.y = SCREEN_HEIGHT / 3;
	}

	void SetisUse(bool enable)
	{
		m_isUse = enable;
	}
};
