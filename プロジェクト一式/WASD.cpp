#include "main.h"
#include "renderer.h"
#include "WASD.h"
#include "Scene.h"
#include "manager.h"
#include "model.h"
#include "Coin.h"
#include "UIdefineList.h"
#include "Input.h"

void WASD::Init()
{
	VERTEX_3D vertex[4];
	m_TexPos[0].x = WASD_WIDTH + WASD_WIDTH / 2;
	m_TexPos[0].y = WASD_HEIGHT / 2;

	m_TexPos[1].x = WASD_WIDTH / 2;
	m_TexPos[1].y = WASD_HEIGHT + WASD_HEIGHT / 2;

	m_TexPos[2].x = WASD_WIDTH + +WASD_WIDTH / 2;
	m_TexPos[2].y = WASD_HEIGHT + WASD_HEIGHT / 2;

	m_TexPos[3].x = WASD_WIDTH * 2 + +WASD_WIDTH / 2;
	m_TexPos[3].y = WASD_HEIGHT + WASD_HEIGHT / 2;

	vertex[0].Position = D3DXVECTOR3(m_TexPos[0].x - WASD_WIDTH / 2, m_TexPos[0].y - WASD_HEIGHT / 2, 0.0f);			//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(m_TexPos[0].x + WASD_WIDTH / 2, m_TexPos[0].y - WASD_HEIGHT / 2, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_TexPos[0].x - WASD_WIDTH / 2, m_TexPos[0].y + WASD_HEIGHT / 2, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_TexPos[0].x + WASD_WIDTH / 2, m_TexPos[0].y + WASD_HEIGHT / 2, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/WASD.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);	//textureの読み込みが失敗した場合エラーを出す仕組み

	//コンパイル済みのシェーダを読み込んでいる
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	//コンパイル済みのシェーダを読み込んでpixelshaderを作っている
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}

void WASD::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void WASD::Update()
{
	if (Keyboard_IsPress(DIK_W))
	{
		m_isTexPress[0] = true;
	}
	if (Keyboard_IsPress(DIK_A))
	{
		m_isTexPress[1] = true;
	}
	if (Keyboard_IsPress(DIK_S))
	{
		m_isTexPress[2] = true;
	}
	if (Keyboard_IsPress(DIK_D))
	{
		m_isTexPress[3] = true;
	}

	if (Keyboard_IsRelease(DIK_W))
	{
		m_isTexPress[0] = false;
	}
	if (Keyboard_IsRelease(DIK_A))
	{
		m_isTexPress[1] = false;
	}
	if (Keyboard_IsRelease(DIK_S))
	{
		m_isTexPress[2] = false;
	}
	if (Keyboard_IsRelease(DIK_D))
	{
		m_isTexPress[3] = false;
	}
}

void WASD::Draw()
{
	for (int i = 0; i < 4; i++)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(m_TexPos[i].x - WASD_WIDTH / 2, m_TexPos[i].y - WASD_HEIGHT / 2, 0.0f);			//座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
		vertex[0].TexCoord = D3DXVECTOR2(0.0f + i * 0.25f, 0.0f);				//テクスチャ座標

		vertex[1].Position = D3DXVECTOR3(m_TexPos[i].x + WASD_WIDTH / 2, m_TexPos[i].y - WASD_HEIGHT / 2, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(0.0f + i * 0.25f + 0.25f, 0.0f);

		vertex[2].Position = D3DXVECTOR3(m_TexPos[i].x - WASD_WIDTH / 2, m_TexPos[i].y + WASD_HEIGHT / 2, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f + i * 0.25f, 1.0f);

		vertex[3].Position = D3DXVECTOR3(m_TexPos[i].x + WASD_WIDTH / 2, m_TexPos[i].y + WASD_HEIGHT / 2, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].TexCoord = D3DXVECTOR2(0.0f + i * 0.25f + 0.25f, 1.0f);

		if (m_isTexPress[i] == true)
		{
			vertex[0].Diffuse = COLOR_ORANGE;
			vertex[1].Diffuse = COLOR_ORANGE;
			vertex[2].Diffuse = COLOR_ORANGE;
			vertex[3].Diffuse = COLOR_ORANGE;
		}

		if (m_isTexPress[i] == false)
		{
			vertex[0].Diffuse = COLOR_WHITE;
			vertex[1].Diffuse = COLOR_WHITE;
			vertex[2].Diffuse = COLOR_WHITE;
			vertex[3].Diffuse = COLOR_WHITE;
		}

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}