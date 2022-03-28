#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "WireTarget.h"
#include "MeshField.h"
#include "OBB.h"
#include "Input.h"
#include "Player.h"
#include "Building.h"

void WireTarget::Init()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sphere.obj");	//\\2個に注意「//」でもいい

	Scene* scene = Manager::GetScene();
	m_Position = D3DXVECTOR3(-3.0f, 6.0f, 3.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.125f, 0.125f, 0.125f);

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());
	m_obb->SetLen_W(0, 0.125f);
	m_obb->SetLen_W(1, 0.125f);
	m_obb->SetLen_W(2, 0.125f);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}
void WireTarget::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void WireTarget::Update()
{	
	Scene* scene = Manager::GetScene();
	SCENE_NAME name = scene->GetSceneName();
	std::vector<Building*> BuildingList = scene->GetGameObjects<Building>(OBJ3D);
	if (name == SCENE_TITLE)
	{
		for (Building* bldg : BuildingList)
		{
			m_Position.x = bldg->GetPos().x;
			m_Position.y = bldg->GetPos().y + 10.0f;
			m_Position.z = bldg->GetPos().z - 5.0f;
		}
	}
	if (name == SCENE_TUTORIAL || name == SCENE_GAMESCENE)
	{
		for (Building* bldg : BuildingList)
		{
			m_Position = bldg->CheckWallMeshToRay(this);
		}
	}

	UpdateOBB();
}

void WireTarget::Draw()
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
	m_TargetMatrix = scale * rot * trans;

	Renderer::SetWorldMatrix(&m_TargetMatrix);

	m_Model->Draw();
}

void WireTarget::UpdateOBB()
{
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y/* + m_obb->GetLen_W(1)*/, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);
}
