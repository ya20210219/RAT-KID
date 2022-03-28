#pragma once

#include "GameObject.h"

class RadarDot : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	bool	m_isUse;
	class GameObject* m_MyObj;
	D3DXVECTOR4		m_MyColor;
	bool		m_isShow;

public:
	//Polygon2D();	//今回は不要
	//~Polygon2D();

	void Init();
	void UnInit();
	void Update();
	void Draw();

	bool GetisUse()
	{
		return m_isUse;
	}

	void SetisUse(bool use)
	{
		m_isUse = use;
	}

	void SetMyObj(GameObject* obj)
	{
		m_MyObj = obj;
	}

	GameObject* GetMyObj()
	{
		return m_MyObj;
	}

	void SetDotColor(D3DXVECTOR4 color)
	{
		m_MyColor = color;
	}
	void SetDotShow(bool enable)
	{
		m_isShow = enable;
	}
};