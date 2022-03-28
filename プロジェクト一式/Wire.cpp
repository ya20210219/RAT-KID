#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "polygon2D.h"
#include "Wire.h"
#include "Scene.h"
#include "manager.h"
#include "Camera.h"
#include "OBB.h"
#include "Player.h"
#include "WireTarget.h"

void Wire::Init()
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/white.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);	//textureの読み込みが失敗した場合エラーを出す仕組み

	//コンパイル済みのシェーダを読み込んでいる
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	//コンパイル済みのシェーダを読み込んでpixelshaderを作っている
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");


	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_WireWidth = WIRE_WIDTH;
}

void Wire::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Wire::Update()
{
	m_WireWidth = WIRE_WIDTH;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Wire::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* cam = scene->GetGameObject<Camera>(SYSTEM);
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	WireTarget * wt = scene->GetGameObject<WireTarget>(OBJ3D);

	if (player->GetisWire() == true && player->GetisCol() == false)
	{
		float addY = player->GetOBB()->GetLen_W(1);
		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		UpdateWireVertexPos(player->GetPos(), wt->GetPos(), cam->GetPos());

		vertex[0].Position = D3DXVECTOR3(wt->GetPos().x, wt->GetPos().y, wt->GetPos().z);
		vertex[0].Position -= -m_WireVec * m_WireWidth;
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
		vertex[0].Diffuse = COLOR_GRAY;

		vertex[1].Position = D3DXVECTOR3(wt->GetPos().x, wt->GetPos().y, wt->GetPos().z);
		vertex[1].Position -= +m_WireVec * m_WireWidth;
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
		vertex[1].Diffuse = COLOR_GRAY;

		vertex[2].Position = D3DXVECTOR3(player->GetPos().x, player->GetPos().y + addY * 2, player->GetPos().z);
		vertex[2].Position -= -m_WireVec * m_WireWidth;
		vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[2].Diffuse = COLOR_GRAY;

		vertex[3].Position = D3DXVECTOR3(player->GetPos().x, player->GetPos().y + addY * 2, player->GetPos().z);
		vertex[3].Position -= +m_WireVec * m_WireWidth;
		vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		vertex[3].Diffuse = COLOR_GRAY;

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		D3DXMATRIX world, scale, trans, rot;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //Yaw = y Pitch = x Roll = z
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		D3DXMatrixIdentity(&world);
		Renderer::SetWorldMatrix(&world);

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Renderer::SetMaterial(material);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

void Wire::UpdateWireVertexPos(D3DXVECTOR3 pPos, D3DXVECTOR3 wPos, D3DXVECTOR3 cPos)
{
	D3DXVECTOR3 PtoWVec = wPos - pPos;
	D3DXVECTOR3 WtoCVec = wPos - cPos;
	
	D3DXVec3Cross(&m_WireVec, &WtoCVec, &PtoWVec);
	D3DXVec3Normalize(&m_WireVec, &m_WireVec);
}
