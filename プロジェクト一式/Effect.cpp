#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "polygon2D.h"
#include "Effect.h"
#include "Scene.h"
#include "manager.h"
#include "Camera.h"

void Effect::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	VERTEX_3D vertex[4];	//頂点を作るための処理

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);		//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//頂点バッファのサイズ(4頂点分のバイト数)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//これが頂点バッファであるという証明
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[EFFECT_COIN]);
	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		COIN_EFFECT_FILE,
		NULL,
		NULL,
		&m_Texture[EFFECT_COIN],
		NULL);	   
	assert(m_Texture[EFFECT_COIN]);	//textureの読み込みが失敗した場合エラーを出す仕組み

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[EFFECT_EXPLOSION]);
	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		EXPLOSION_EFFECT_FILE,
		NULL,
		NULL,
		&m_Texture[EFFECT_EXPLOSION],
		NULL);
	assert(m_Texture[EFFECT_EXPLOSION]);	//textureの読み込みが失敗した場合エラーを出す仕組み
	//コンパイル済みのシェーダを読み込んでいる


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	//コンパイル済みのシェーダを読み込んでpixelshaderを作っている
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	m_Frame = 0;
	m_Cnt = 0;
}

void Effect::UnInit()
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		m_VertexBuffer[i]->Release();
		m_Texture[i]->Release();
	}

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Effect::Update()
{
	m_Cnt++;
	if (m_Cnt > 2)
	{
		m_Frame++;
		m_Cnt = 0;
	}

	m_Maxframe = SetMaxFrame(m_EffectType);
	if (m_Frame > m_Maxframe)
	{
		m_Frame = 0;
		SetDestroy();
	}
}

void Effect::Draw()
{
	D3DXVECTOR2 uv;

	uv = SetUV(m_EffectType);

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer[static_cast<int>(m_EffectType)], 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex = SetVertexData(m_EffectType, vertex, uv);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer[static_cast<int>(m_EffectType)], 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(SYSTEM);
	D3DXMATRIX view = camera->GetViewMatrix();

	// 逆行列を求める
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);

	// 平行移動の部分は無視する
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//マトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[static_cast<int>(m_EffectType)], &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[static_cast<int>(m_EffectType)]);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

D3DXVECTOR2 Effect::SetUV(EFFECT_LIST type)
{
	D3DXVECTOR2 uv;
	switch (type)
	{
	case EFFECT_COIN:
		uv.x = m_Frame % MAX_COIN_ANIME_W * (1.0f / MAX_COIN_ANIME_W);
		uv.y = m_Frame / MAX_COIN_ANIME_W * (1.0f / MAX_COIN_ANIME_H);
		break;	
	case EFFECT_EXPLOSION:
		uv.x = m_Frame % MAX_EXPLOSION_ANIME_W * (1.0f / MAX_EXPLOSION_ANIME_W);
		uv.y = m_Frame / MAX_EXPLOSION_ANIME_W * (1.0f / MAX_EXPLOSION_ANIME_H);
		break;
	default:
		break;
	}

	return uv;
}

int Effect::SetMaxFrame(EFFECT_LIST type)
{
	int frame;
	switch (type)
	{
	case EFFECT_COIN:
		frame = (MAX_COIN_ANIME_W * MAX_COIN_ANIME_H) - 1;
		break;
	case EFFECT_EXPLOSION:
		frame = (MAX_EXPLOSION_ANIME_W * MAX_EXPLOSION_ANIME_H) - 1;
		break;
	default:
		break;
	}

	return frame;
}

VERTEX_3D* Effect::SetVertexData(EFFECT_LIST type, VERTEX_3D* vertex, D3DXVECTOR2 uv)
{
	switch (type)
	{
	case EFFECT_COIN:
		vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);		//座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//法線ベクトル
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
		vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y);						//テクスチャ座標

		vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(uv.x + (1.0f / MAX_COIN_ANIME_W), uv.y);

		vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + (1.0f / MAX_COIN_ANIME_H));

		vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(uv.x + (1.0f / MAX_COIN_ANIME_W), uv.y + (1.0f / MAX_COIN_ANIME_H));
		break;
	case EFFECT_EXPLOSION:
		vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);		//座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//法線ベクトル
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
		vertex[0].TexCoord = D3DXVECTOR2(uv.x, uv.y);						//テクスチャ座標

		vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(uv.x + (1.0f / MAX_EXPLOSION_ANIME_W), uv.y);

		vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(uv.x, uv.y + (1.0f / MAX_EXPLOSION_ANIME_H));

		vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(uv.x + (1.0f / MAX_EXPLOSION_ANIME_W), uv.y + (1.0f / MAX_EXPLOSION_ANIME_H));
		break;
	default:
		break;
	}

	return vertex;
}

void Effect::SetEffectAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, EFFECT_LIST type)
{
	m_Position = pos;
	m_Rotation = rot;
	m_Scale = scl;
	m_EffectType = type;
}
