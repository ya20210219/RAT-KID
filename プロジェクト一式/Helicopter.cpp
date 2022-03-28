#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "GameObject.h"
#include "Helicopter.h"
#include "Calculation.h"
#include "MeshField.h"
#include "Building.h"
#include "OBB.h"
#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "Score.h"
#include "Effect.h"
#include "RadarDot.h"
#include "audio.h"
#include "WireTarget.h"

Helicopter::Propeller propeller[2];
Model* Helicopter::m_Model;
Model* Helicopter::Propeller::m_Model[2];

void Helicopter::Init()
{
	Scene* scene = Manager::GetScene();
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	m_Speed = HELICOPTER_SPEED;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());
	m_obb->SetLen_W(0, 4.0f);
	m_obb->SetLen_W(1, 2.0f);
	m_obb->SetLen_W(2, 4.0f);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(m_Rotation.x, m_Rotation.y, m_Rotation.z),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);

	propeller[0].Pos = D3DXVECTOR3(m_Position.x - 0.4f, m_Position.y + 1.75f, m_Position.z);
	propeller[0].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[0].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	propeller[1].Pos = D3DXVECTOR3(m_Position.x + 5.5f, m_Position.y + 0.25f, m_Position.z - 0.1f);
	propeller[1].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[1].Scl = D3DXVECTOR3(0.25f, 0.25f, 0.25f);

	m_ExpSE = scene->AddGameObject<Audio>(SYSTEM);
	m_ExpSE->Load(SE_EXPLOSION);
	m_PropellerSE = scene->AddGameObject<Audio>(SYSTEM);
	m_PropellerSE->Load(SE_DRONE);
	m_PropellerSE->Play(true, 0.1f);
	m_Direction = GetForward();

	m_OldPosition = m_Position;
}

void Helicopter::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_obb->SetDestroy();
}

void Helicopter::Update()
{
	Scene* scene = Manager::GetScene();
	SCENE_NAME name = scene->GetSceneName();

	if (name == SCENE_GAMESCENE || name == SCENE_TUTORIAL)
	{
		UpdateOBB();
		UpdateMove();
		UpdateSound();
		//旧座標更新
		m_OldPosition = m_Position;
	}
}

void Helicopter::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* cam = scene->GetGameObject<Camera>(0);

	if (!cam->CheckView(m_Position, m_obb))
	{
		return;
	}

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	m_EnemyMatrix = scale * rot * trans;

	Renderer::SetWorldMatrix(&m_EnemyMatrix);

	m_Model->Draw();

	for (int i = 0; i < 2; i++)
	{
		propeller[i].Draw(i, m_EnemyMatrix);
	}
}

void Helicopter::Propeller::Draw(int index, D3DMATRIX matrix)
{
	D3DXMATRIX scale, rot, trans, DMatrix;

	//マトリクス設定
	D3DXMatrixScaling(&scale, propeller[index].Scl.x, propeller[index].Scl.y, propeller[index].Scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, propeller[index].Rot.y, propeller[index].Rot.x, propeller[index].Rot.z); //Yaw = y Pitch = x Roll = z
	D3DXMatrixTranslation(&trans, propeller[index].Pos.x, propeller[index].Pos.y, propeller[index].Pos.z);

	propeller[index].m_PropellerMatrix = scale * rot * trans * matrix;

	Renderer::SetWorldMatrix(&propeller[index].m_PropellerMatrix);

	propeller[index].m_Model[index]->Draw();
}

void Helicopter::UpdateMove()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	   	
	if (MeasureDist(m_Position, GOAL_POS) > 1.0f)
	{
		D3DXVec3Lerp(&m_Rotation, &m_Rotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_Speed / MeasureDist(m_Position, GOAL_POS));
		m_Direction = GOAL_POS - m_Position;
		D3DXVec3Normalize(&m_Direction, &m_Direction);

		m_Position += m_Direction * m_Speed;
	}

	propeller[0].Rot.y += 1.0f;
	AdjustmentRadian(propeller[0].Rot.y);
	propeller[1].Rot.z += 1.0f;
	AdjustmentRadian(propeller[1].Rot.z);
}

void Helicopter::UpdateOBB()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	std::vector<Building*> BuildingList = scene->GetGameObjects<Building>(OBJ3D);
	Score* score = scene->GetGameObject<Score>(OBJ2D);

	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(m_Rotation.x, m_Rotation.y, m_Rotation.z),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);
}

void Helicopter::UpdateHeight()
{
	Scene* scene = Manager::GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>(OBJ3D);

	float y = meshField->GetHeight(m_Position);
	if (m_Position.y - m_obb->GetLen_W(1) < y)
	{
		//高さを床に合わせる
		m_Position.y = y + m_obb->GetLen_W(1);
	}
}

void Helicopter::UpdateSound()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	//プロペラ音調整
	if (MeasureDist(player->GetPos(), m_Position) > MAX_HELI_SE_DIST)
	{
		m_PropellerSE->SetVolume(0.0f);
	}

	else
	{
		m_PropellerSE->SetVolume(std::min(1.0f,(MAX_HELI_SE_DIST - (MeasureDist(player->GetPos(), m_Position))) / 10));
	}
}

void Helicopter::CheckMeshToRay()
{
	float RayDistance;
	float RecentRayDistance = 1000.0f;
	bool bRayHit;
	D3DXVECTOR3 wPos[3];
	D3DXVECTOR3 wDir;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	Camera* cam = scene->GetGameObject<Camera>(SYSTEM);
	WireTarget* wt = scene->GetGameObject<WireTarget>(OBJ3D);

	unsigned int maxnum;
	maxnum = m_Model->GetIndexNum();

	for (unsigned int i = 0; i < maxnum; i += 3)
	{
		VERTEX_3D *vertex = m_Model->GetVertexArray();
		unsigned int *nownum = m_Model->GetIndexArray();

		D3DXVECTOR3 pos0;
		pos0.x = (vertex + *(nownum + (i + 0)))->Position.x + m_Position.x;
		pos0.y = (vertex + *(nownum + (i + 0)))->Position.y + m_Position.y;
		pos0.z = (vertex + *(nownum + (i + 0)))->Position.z + m_Position.z;
		D3DXVECTOR3 pos1;
		pos1.x = (vertex + *(nownum + (i + 1)))->Position.x + m_Position.x;
		pos1.y = (vertex + *(nownum + (i + 1)))->Position.y + m_Position.y;
		pos1.z = (vertex + *(nownum + (i + 1)))->Position.z + m_Position.z;
		D3DXVECTOR3 pos2;
		pos2.x = (vertex + *(nownum + (i + 2)))->Position.x + m_Position.x;
		pos2.y = (vertex + *(nownum + (i + 2)))->Position.y + m_Position.y;
		pos2.z = (vertex + *(nownum + (i + 2)))->Position.z + m_Position.z;

		//レイの開始点
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(cam->GetPos().x, cam->GetPos().y, cam->GetPos().z, 0.0f);

		//レイを飛ばす方向
		DirectX::XMVECTOR dir = DirectX::XMVectorSet(cam->GetDirection().x, cam->GetDirection().y, cam->GetDirection().z, 0.0f);

		DirectX::XMVECTOR v0 = DirectX::XMVectorSet(pos0.x, pos0.y, pos0.z, 0.0f);
		DirectX::XMVECTOR v1 = DirectX::XMVectorSet(pos1.x, pos1.y, pos1.z, 0.0f);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSet(pos2.x, pos2.y, pos2.z, 0.0f);

		bRayHit = DirectX::TriangleTests::Intersects(pos, DirectX::XMVector3Normalize(dir), v0, v1, v2, RayDistance);
		if (bRayHit)
		{
			//すべての三角形を調べて、一番近い所を採用
			//当たった点との距離で一番短い物を衝突点との距離として採用する			
			if (player->GetRecentRayDistance() >= RayDistance)
			{
				player->SetRecentRayDistance(RayDistance);
			}
			if (Mouse_IsLeftPress() == false)
			{
				D3DXVECTOR3 pos = cam->GetPos() + cam->GetDirection() * player->GetRecentRayDistance();
				player->SetTargetPos(pos);
				wt->SetTargetPos(pos);

				D3DXVECTOR3 pDir = pos - player->GetPos();
				D3DXVec3Normalize(&pDir, &pDir);
				wPos[0] = D3DXVECTOR3(pos0.x, pos0.y, pos0.z);
				wPos[1] = D3DXVECTOR3(pos1.x, pos1.y, pos1.z);
				wPos[2] = D3DXVECTOR3(pos2.x, pos2.y, pos2.z);

				D3DXVECTOR3 v10 = wPos[1] - wPos[0];
				D3DXVECTOR3 v12 = wPos[2] - wPos[1];

				wDir = *D3DXVec3Cross(&wDir, &v10, &v12);
				D3DXVec3Normalize(&wDir, &wDir);
				wt->SetTargetMeshDirection(wDir);
			}
		}
	}
}