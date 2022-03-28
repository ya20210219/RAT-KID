#pragma once

#include "GameObject.h"

#define MAX_EXPLOSION_ANIME_W (8)   //横アニメーション数(アニメーションの絵(画像)の数)
#define MAX_EXPLOSION_ANIME_H (1)   //縦アニメーション数(アニメーションの絵(画像)の数)

#define MAX_COIN_ANIME_W (5)   //横アニメーション数(アニメーションの絵(画像)の数)
#define MAX_COIN_ANIME_H (2)   //縦アニメーション数(アニメーションの絵(画像)の数)


#define EXPLOSION_EFFECT_FILE "asset/texture/HitEffect.jpg"
#define COIN_EFFECT_FILE "asset/texture/pipofm-topen01_480.png"


enum EFFECT_LIST
{
	EFFECT_COIN,
	EFFECT_EXPLOSION,

	EFFECT_MAX
};

class Effect : public GameObject
{
private:

	ID3D11Buffer*				m_VertexBuffer[EFFECT_MAX] = { NULL };	//リリースするタイミングで中身が無いことを確認してる
	ID3D11ShaderResourceView*	m_Texture[EFFECT_MAX] = { NULL };

	ID3D11VertexShader*			m_VertexShader = NULL;
	ID3D11PixelShader*			m_PixelShader = NULL;
	ID3D11InputLayout*			m_VertexLayout = NULL;

	int	m_Cnt;
	int m_Frame;
	int m_Maxframe;
	EFFECT_LIST m_EffectType;
public:

	void Init();
	void UnInit();
	void Update();
	void Draw();
	D3DXVECTOR2 SetUV(EFFECT_LIST type);
	int SetMaxFrame(EFFECT_LIST type);
	VERTEX_3D* SetVertexData(EFFECT_LIST type, VERTEX_3D* vertex, D3DXVECTOR2 uv);
	void SetEffectAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, EFFECT_LIST type);

	void SetEffectName(EFFECT_LIST type)
	{
		m_EffectType = type;
	}
};