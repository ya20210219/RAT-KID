#pragma once

#include "GameObject.h"

#define MAX_MAP_WIDTH	(40)
#define MAX_MAP_HEIGHT	(40)

class MeshField : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11Buffer*				m_IndexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	VERTEX_3D	m_Vertex[MAX_MAP_WIDTH + 1][MAX_MAP_HEIGHT + 1];

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;
public:

	void Init();
	void UnInit();
	void Update();
	void Draw();

	float GetHeight(D3DXVECTOR3 pos);
	D3DXVECTOR3 RightVec(D3DXVECTOR3 pos, D3DXVECTOR3 v1);
	D3DXVECTOR3 MeshFieldWithinRange(D3DXVECTOR3 pos);
};