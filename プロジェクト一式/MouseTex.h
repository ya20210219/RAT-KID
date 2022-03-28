#pragma once

#include "GameObject.h"

class MouseTex : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;	//�����[�X����^�C�~���O�Œ��g���������Ƃ��m�F���Ă�
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	D3DXVECTOR2 m_TexPos[3];
	bool		m_isTexPress[3];

public:

	void Init();
	void UnInit();
	void Update();
	void Draw();
};

