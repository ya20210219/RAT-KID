#pragma once

#include "GameObject.h"
#define ENEMY_SPEED (0.05f)
#define ADJUSTMENT_Y (0.8f)
#define MAX_SE_DIST (12.5f)

class Enemy : public GameObject
{
private:
	static Model* m_Model;
	class RadarDot* m_Dot;
	class Audio* m_ExpSE;
	class Audio* m_PropellerSE;

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_OldPosition;

	D3DXMATRIX m_EnemyMatrix;

	float m_TargetDist;
	float m_TargetRotY;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw(); 
	
	class Propeller
	{
	public:
		static Model* m_Model;

		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scl;

		D3DXMATRIX m_PropellerMatrix;

		void Draw(int index, D3DMATRIX matrix);

		static void Load()
		{
			m_Model = new Model();
			m_Model->Load("asset\\model\\propeller.obj");
		};
		static void UnLoad()
		{
			m_Model->Unload();
			delete m_Model;
		};
	};

	void UpdateMove();
	void UpdateOBB();
	void UpdateHeight();
	void UpdateSound();
	void UpdateTitleMove();

	static void Load()
	{
		m_Model = new Model();
		m_Model->Load("asset\\model\\Enemy2.obj");	//\\2ŒÂ‚É’ˆÓu//v‚Å‚à‚¢‚¢
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

