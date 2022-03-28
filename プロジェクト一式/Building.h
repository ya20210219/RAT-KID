#pragma once

#include "GameObject.h"
#define MAX_BLDG_TYPE (3)
#define NOMAL_EDGE (0.9f)
#define LARGE_EDGE (1.2f)
#define HUGE_EDGE (1.5f)

enum BldgType
{
	BLDG_TYPE_NORMAL,
	BLDG_TYPE_LARGE,
	BLDG_TYPE_HUGE,
};


class Building : public GameObject
{
private:
	static Model* m_Model[3];
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_Speed;
	D3DXVECTOR3 m_OldPosition;

	D3DXMATRIX m_EnemyMatrix;

	int m_Frame;
	float m_BlendRate;
	float m_AdjustmentEdgeHeight;

	bool	m_isCorrection;

	BldgType m_Type;

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	void UpdateHeight();
	void UpdateOBB();

	static void Load()
	{
		m_Model[0] = new Model();
		m_Model[0]->Load("asset\\model\\TestBldg2.obj");	//\\2個に注意「//」でもいい
		m_Model[1] = new Model();
		m_Model[1]->Load("asset\\model\\TestBldg5.obj");	//\\2個に注意「//」でもいい
		m_Model[2] = new Model();
		m_Model[2]->Load("asset\\model\\TestBldg6.obj");	//\\2個に注意「//」でもいい
	};
	static void UnLoad()
	{
		for (int i = 0; i < MAX_BLDG_TYPE; i++)
		{
			m_Model[i]->Unload();
			delete m_Model[i];
		}
	};

	float GetAdjustmentEdgeHeight()
	{
		return m_AdjustmentEdgeHeight;
	}

	Model* GetModel(int num)
	{
		return m_Model[num];
	}

	void CheckMeshToRay();
	D3DXVECTOR3 CheckMeshToCam();
	bool CheckFootMeshToRay(D3DXVECTOR3 dir);

	D3DXVECTOR3 CheckWallMeshToRay(GameObject* obj);

	void CreateBldg(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BldgType t);
};
