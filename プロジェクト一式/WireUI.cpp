#include "main.h"
#include "renderer.h"
#include "GameObject.h"
#include "polygon2D.h"
#include "WireUI.h"
#include "Scene.h"
#include "manager.h"
#include "Camera.h"
#include "WireTarget.h"
#include "Calculation.h"

void WireUI::Init()
{
	m_Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	VERTEX_3D vertex[4];	//頂点を作るための処理

	vertex[0].Position = D3DXVECTOR3(m_Position.x - 50.0f, m_Position.y - 50.0f, m_Position.z);			//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(m_Position.x + 50.0f, m_Position.y - 50.0f, m_Position.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_Position.x - 50.0f, m_Position.y + 50.0f, m_Position.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_Position.x + 50.0f, m_Position.y + 50.0f, m_Position.z);
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
		WIRE_UI_FILE,
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	//コンパイル済みのシェーダを読み込んでpixelshaderを作っている
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void WireUI::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void WireUI::Update()
{
	SetScreenPos();
}

void WireUI::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(m_Position.x - 50.0f, m_Position.y - 50.0f, m_Position.z);			//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(m_Position.x + 50.0f, m_Position.y - 50.0f, m_Position.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_Position.x - 50.0f, m_Position.y + 50.0f, m_Position.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_Position.x + 50.0f, m_Position.y + 50.0f, m_Position.z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

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

void WireUI::SetScreenPos()
{
	Scene* scene = Manager::GetScene();
	WireTarget* wt = scene->GetGameObject<WireTarget>(OBJ3D);
	Camera* cam = scene->GetGameObject<Camera>(SYSTEM);

	DirectX::XMVECTOR wPos = DirectX::XMVectorSet(wt->GetPos().x, wt->GetPos().y, wt->GetPos().z, 0.0f);
	DirectX::XMMATRIX cPMat = DirectX::XMMatrixSet(cam->GetProjectionMatrix()._11, cam->GetProjectionMatrix()._12, cam->GetProjectionMatrix()._13, cam->GetProjectionMatrix()._14,
		cam->GetProjectionMatrix()._21, cam->GetProjectionMatrix()._22, cam->GetProjectionMatrix()._23, cam->GetProjectionMatrix()._24,
		cam->GetProjectionMatrix()._31, cam->GetProjectionMatrix()._32, cam->GetProjectionMatrix()._33, cam->GetProjectionMatrix()._34,
		cam->GetProjectionMatrix()._41, cam->GetProjectionMatrix()._42, cam->GetProjectionMatrix()._43, cam->GetProjectionMatrix()._44);


	DirectX::XMMATRIX cVMat = DirectX::XMMatrixSet(cam->GetViewMatrix()._11, cam->GetViewMatrix()._12, cam->GetViewMatrix()._13, cam->GetViewMatrix()._14,
		cam->GetViewMatrix()._21, cam->GetViewMatrix()._22, cam->GetViewMatrix()._23, cam->GetViewMatrix()._24,
		cam->GetViewMatrix()._31, cam->GetViewMatrix()._32, cam->GetViewMatrix()._33, cam->GetViewMatrix()._34,
		cam->GetViewMatrix()._41, cam->GetViewMatrix()._42, cam->GetViewMatrix()._43, cam->GetViewMatrix()._44);


	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();

	DirectX::XMVECTOR pos = DirectX::XMVector3Project(wPos,
		0, 0,
		static_cast<float>SCREEN_WIDTH, static_cast<float>SCREEN_HEIGHT,
		0.0f, 1.0f,
		cPMat, cVMat, world);

	m_Position.x = DirectX::XMVectorGetX(pos);
	m_Position.y = DirectX::XMVectorGetY(pos);
	m_Position.z = 0.0f;
}