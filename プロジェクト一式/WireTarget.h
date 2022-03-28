#pragma once

#include "GameObject.h"

class WireTarget : public GameObject
{
private:
	class Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_TargetMeshDirection;

	D3DXMATRIX m_TargetMatrix;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
	void UpdateOBB();

	void SetTargetPos(D3DXVECTOR3 pos)
	{
		m_Position = pos;
	}

	void SetTargetMeshDirection(D3DXVECTOR3 dir)
	{
		m_TargetMeshDirection = dir;
	}

	D3DXVECTOR3 GetTargetMeshDirection()
	{
		return m_TargetMeshDirection;
	}
};

