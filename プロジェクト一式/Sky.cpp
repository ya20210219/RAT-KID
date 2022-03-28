#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "GameObject.h"
#include "Sky.h"
#include "Calculation.h"
#include "MeshField.h"
#include "Building.h"

void Sky::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sky.obj");	//\\2個に注意「//」でもいい

	Scene* scene = Manager::GetScene();
	m_Position = D3DXVECTOR3(0.0f, -0.01f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, D3DX_PI - (D3DX_PI / 3), 0.0f);
	m_Scale = D3DXVECTOR3(200.0f, 200.0f, 200.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void Sky::UnInit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Sky::Update()
{
	//高さ更新、滑りチェック

}

void Sky::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //Yaw = y Pitch = x Roll = z
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_SkyMatrix = scale * rot * trans;

	Renderer::SetWorldMatrix(&m_SkyMatrix);

	m_Model->Draw();
}