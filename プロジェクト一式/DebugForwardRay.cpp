#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "polygon2D.h"
#include "DebugForwardRay.h"
#include "Scene.h"
#include "manager.h"
#include "Camera.h"
#include "Player.h"
#include "DebugSystem.h"

void DebugForwardRay::Init()
{
	VERTEX_3D vertex[2];	//頂点を作るための処理

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);		//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 2;	//頂点バッファのサイズ(4頂点分のバイト数)
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
}

void DebugForwardRay::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void DebugForwardRay::Update()
{

}

void DebugForwardRay::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(SYSTEM);
	DebugSystem* d = scene->GetGameObject<DebugSystem>(SYSTEM);
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	if (d->GetDebug() == true)
	{
		D3DXVECTOR3 dirP = player->GetPos() - player->GetDirection() * 2.0f;
		dirP.y += 1.0f;
		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3(player->GetPos().x, player->GetPos().y + 1.0f, player->GetPos().z);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
		vertex[0].Diffuse = COLOR_GREEN;

		vertex[1].Position = D3DXVECTOR3(dirP.x, dirP.y, dirP.z);		//座標
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//法線ベクトル
		vertex[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標
		vertex[1].Diffuse = COLOR_GREEN;

		//vertex[0].Position = D3DXVECTOR3(18.0f, 1.0f, 0.0f);		//座標
		//vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//法線ベクトル
		//vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標
		//vertex[0].Diffuse = COLOR_GREEN;

		//vertex[1].Position = D3DXVECTOR3(25.0f, 1.0f, 0.0f);
		//vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//vertex[1].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		//vertex[1].Diffuse = COLOR_GREEN;

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
		D3DXMATRIX view = camera->GetViewMatrix();

		//// 逆行列を求める
		//D3DXMATRIX invView;
		//D3DXMatrixInverse(&invView, NULL, &view);

		//// 平行移動の部分は無視する
		//invView._41 = 0.0f;
		//invView._42 = 0.0f;
		//invView._43 = 0.0f;

		//マトリクス設定
		D3DXMATRIX world, scale, trans, rot;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //Yaw = y Pitch = x Roll = z
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		//world = scale * invView * trans;
		world = scale * rot * trans;
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
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(2, 0);
	}
}
