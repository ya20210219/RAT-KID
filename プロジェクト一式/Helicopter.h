#pragma once

#include "GameObject.h"
#define HELICOPTER_SPEED (0.5f)
#define ADJUSTMENT_Y (0.8f)
#define MAX_HELI_SE_DIST (50.0f)
#define MAX_HELI_DIST (5.0f)
#define GOAL_POS (D3DXVECTOR3(0.0f, 90.0f, -60.0f))

class Helicopter : public GameObject
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
		static Model* m_Model[2];

		D3DXVECTOR3 Pos;
		D3DXVECTOR3 Rot;
		D3DXVECTOR3 Scl;

		D3DXMATRIX m_PropellerMatrix;

		void Draw(int index, D3DMATRIX matrix);

		static void Load()
		{
			m_Model[0] = new Model();
			m_Model[0]->Load("asset\\model\\Heli_propeller.obj");
			m_Model[1] = new Model();
			m_Model[1]->Load("asset\\model\\Heli_propeller2.obj");
		};
		static void UnLoad()
		{
			for (int i = 0; i < 2; i++)
			{
				m_Model[i]->Unload();
				delete m_Model[i];
			}
		};
	};

	void UpdateMove();
	void UpdateOBB();
	void UpdateHeight();
	void UpdateSound();
	void CheckMeshToRay();

	static void Load()
	{
		m_Model = new Model();
		m_Model->Load("asset\\model\\Helicopter.obj");	//\\2ŒÂ‚É’ˆÓu//v‚Å‚à‚¢‚¢
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
