#pragma once

#include "GameObject.h"
#include "model.h"

class OBB : public GameObject
{
private:
	static Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_OBBPos;			// 位置
	D3DXVECTOR3 m_NormaDirect[3];   // 方向ベクトル
	FLOAT m_fLength[3];             // 各軸方向の長さ

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	static void Load()
	{
		m_Model = new Model();
		m_Model->Load(MODEL_CUBE);	//\\2個に注意「//」でもいい
	};
	static void UnLoad()
	{
		m_Model->Unload();
		delete m_Model;
	};

	D3DXVECTOR3 GetDirect(int index) { return m_NormaDirect[index]; }   // 指定軸番号の方向ベクトルを取得
	FLOAT GetLen_W(int index) { return m_fLength[index]; }			  // 指定軸方向の長さを取得
	D3DXVECTOR3 GetPos_W() { return m_OBBPos; }				          // 位置を取得

	void SetOBBPos(D3DXVECTOR3 pos) { m_OBBPos = pos; }
	void SetDirect(int index, D3DXVECTOR3 dir) { m_NormaDirect[index] = dir; }
	void SetLen_W(int index, float len) { m_fLength[index] = len; }

	bool ColOBBs(OBB &obb1, OBB &obb2);

	Model* GetOBBModel()
	{
		return m_Model;
	}
};
