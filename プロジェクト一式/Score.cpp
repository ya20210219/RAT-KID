#include "main.h"
#include "renderer.h"
#include "Score.h"
#include "Scene.h"
#include "manager.h"
#include "UIdefineList.h"
#include "Coin.h"

void Score::Init()
{
	m_Position = D3DXVECTOR3((SCREEN_WIDTH / 2) + TEX_WIDTH + SCORE_WIDTH * MAX_SCORE_DIGIT, SCORE_WIDTH, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	VERTEX_3D vertex[4];	//頂点を作るための処理

	vertex[0].Position = D3DXVECTOR3(m_Position.x - SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);			//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(m_Position.x + SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_Position.x - SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_Position.x + SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
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
		"asset/texture/number.jpg",
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

void Score::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Score::Update()
{
	Scene* scene = Manager::GetScene();

	if (scene->GetSceneName() == SCENE_RESULT)
	{
		m_Position = m_Position = D3DXVECTOR3(SCREEN_WIDTH / 2 + SCORE_WIDTH, SCREEN_HEIGHT / 2 + ADJUSTMENT_SCORE_HEIGHT, 0.0f);
	}
}

void Score::Draw()
{
	Scene* scene = Manager::GetScene();
	int score = scene->GetScore();
	int digit = 0;

	//桁数算出
	do
	{
		score /= 10;
		digit += 1;
		if (digit > MAX_SCORE_DIGIT)
		{
			digit = MAX_SCORE_DIGIT;
		}

	} while (score > 0);

	score = scene->GetScore();

	for (int i = 0; i < digit; i++)
	{
		int num = 0;
		num = score % 10;				//スコアの1の位を抜き出す
		score /= 10;					//スコアを1の位ずらしている

		float x = num % MAX_SCORE_TEX_WIDTH * (1.0f / MAX_SCORE_TEX_WIDTH);
		float y = num / MAX_SCORE_TEX_WIDTH * (1.0f / MAX_SCORE_TEX_HEIGHT);

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) - SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);			//座標
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
		vertex[0].TexCoord = D3DXVECTOR2(x, y);						//テクスチャ座標

		vertex[1].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) + SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / MAX_SCORE_TEX_WIDTH), y);

		vertex[2].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) - SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / MAX_SCORE_TEX_HEIGHT));

		vertex[3].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) + SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / MAX_SCORE_TEX_WIDTH), y + (1.0f / MAX_SCORE_TEX_HEIGHT));


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