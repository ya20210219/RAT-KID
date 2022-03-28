#pragma once

#include "GameObject.h"
#include "Model.h"
#define SCORE_COIN (100)

class Coin : public GameObject
{
private:
	static Model* m_Model;
	class RadarDot* m_Dot;
	class Audio* m_CoinSE;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_Speed;
	D3DXVECTOR3 m_OldPosition;

	D3DXMATRIX m_EnemyMatrix;

	int m_Frame;
	float m_BlendRate;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	void UpdateHeight();
	void UpdateMove();
	void UpdateOBB();

	static void Load()
	{
		m_Model = new Model();
		m_Model->Load("asset\\model\\coin001.obj");	//\\2ŒÂ‚É’ˆÓu//v‚Å‚à‚¢‚¢
	};
	static void UnLoad()
	{
		m_Model->Unload();
		delete m_Model;
	};

	Model* GetModel()
	{
		return m_Model;
	}

	void SetDot(RadarDot* dot)
	{
		m_Dot = dot;
	}

	RadarDot* GetDot()
	{
		return m_Dot;
	}
};
