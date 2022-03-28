#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "GameObject.h"
#include "Enemy.h"
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

Enemy::Propeller propeller[4];

Model* Enemy::m_Model;
Model* Enemy::Propeller::m_Model;

void Enemy::Init()
{
	Scene* scene = Manager::GetScene();
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_Speed = ENEMY_SPEED;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());
	m_obb->SetLen_W(0, 2.0f);
	m_obb->SetLen_W(1, 1.0f);
	m_obb->SetLen_W(2, 2.0f);

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y + ADJUSTMENT_Y, m_obb->GetPos_W().z),
		D3DXVECTOR3(m_Rotation.x, m_Rotation.y, m_Rotation.z),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);

	propeller[0].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (D3DX_PI / 4)) * 3.5f,
								   m_Position.y + 2.85f,
								  (m_Position.z + cos(m_Rotation.y + (D3DX_PI / 4)) * 3.25f));
	propeller[0].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[0].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	propeller[1].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (-D3DX_PI / 4)) * 3.5f,
								   m_Position.y + 2.85f,
								  (m_Position.z + cos(m_Rotation.y + (D3DX_PI / 4)) * 3.25f));
	propeller[1].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[1].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	propeller[2].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (-D3DX_PI / 4)) * 3.5f,
								   m_Position.y + 2.85f,
								  (m_Position.z - cos(m_Rotation.y - (D3DX_PI / 4)) * 3.75f));
	propeller[2].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[2].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	propeller[3].Pos = D3DXVECTOR3(m_Position.x + sinf(m_Rotation.y + (D3DX_PI / 4)) * 3.5f,
							       m_Position.y + 2.85f,
								  (m_Position.z - cos(m_Rotation.y - (D3DX_PI / 4)) * 3.75f));
	propeller[3].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	propeller[3].Scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_ExpSE = scene->AddGameObject<Audio>(SYSTEM);
	m_ExpSE->Load(SE_EXPLOSION);	
	m_PropellerSE = scene->AddGameObject<Audio>(SYSTEM);
	m_PropellerSE->Load(SE_DRONE);
	m_PropellerSE->Play(true, 0.1f);
	m_Direction = GetForward();

	m_OldPosition = m_Position;
}

void Enemy::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_obb->SetDestroy();
}

void Enemy::Update()
{
	Scene* scene = Manager::GetScene();
	SCENE_NAME name = scene->GetSceneName();
	if (name == SCENE_TITLE)
	{
		UpdateTitleMove();
	}
	if (name == SCENE_GAMESCENE)
	{
		UpdateOBB();
		UpdateMove();
		UpdateHeight();
		UpdateSound();
		//旧座標更新
		m_OldPosition = m_Position;
	}
}

void Enemy::Draw()
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

	for (int i = 0; i < 4; i++)
	{
		propeller[i].Draw(i, m_EnemyMatrix);
	}
}

void Enemy::Propeller::Draw(int index, D3DMATRIX matrix)
{
	D3DXMATRIX scale, rot, trans, DMatrix;

	//マトリクス設定
	D3DXMatrixScaling(&scale, propeller[index].Scl.x, propeller[index].Scl.y, propeller[index].Scl.z);
	D3DXMatrixRotationYawPitchRoll(&rot, propeller[index].Rot.y, propeller[index].Rot.x, propeller[index].Rot.z); //Yaw = y Pitch = x Roll = z
	D3DXMatrixTranslation(&trans, propeller[index].Pos.x, propeller[index].Pos.y, propeller[index].Pos.z);

	propeller[index].m_PropellerMatrix = scale * rot * trans * matrix;

	Renderer::SetWorldMatrix(&propeller[index].m_PropellerMatrix);

	propeller[index].m_Model->Draw();
}

void Enemy::UpdateMove()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	m_TargetRotY = (-atan2f((player->GetPos().z - m_Position.z), (player->GetPos().x - m_Position.x)) + (D3DX_PI / 2));

	AdjustmentRadian(m_TargetRotY);

	m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_TargetRotY, 0.5f);

	m_Direction = player->GetPos() - m_Position;
	D3DXVec3Normalize(&m_Direction, &m_Direction);

	m_Position += m_Direction * m_Speed;

	for (int i = 0; i < 4; i++)
	{
		propeller[i].Rot.y += 2.5f;
		AdjustmentRadian(propeller[i].Rot.y);
	}
}

void Enemy::UpdateOBB()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	std::vector<Building*> BuildingList = scene->GetGameObjects<Building>(OBJ3D);
	std::vector<Enemy*> EnemyList = scene->GetGameObjects<Enemy>(OBJ3D);
	Score* score = scene->GetGameObject<Score>(OBJ2D);

	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y/* + m_obb->GetLen_W(1)*/, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y + ADJUSTMENT_Y, m_obb->GetPos_W().z),
		D3DXVECTOR3(m_Rotation.x, m_Rotation.y, m_Rotation.z),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);

	for (Building* bldg : BuildingList)
	{
		m_Position = bldg->CheckWallMeshToRay(this);
	}

	for (Enemy* enemy : EnemyList)
	{
		if (enemy != this)
		{
			if (m_obb->ColOBBs(*m_obb, *enemy->GetOBB()))
			{
				//当たった敵への方向ベクトル
				m_Direction = m_Position - enemy->GetPos();
				D3DXVec3Normalize(&m_Direction, &m_Direction);
				//方向ベクトルへ進ませる
				m_Position += m_Direction * m_Speed;
			}
		}
	}

	if (m_obb->ColOBBs(*m_obb, *player->GetOBB()))
	{
		m_Dot->SetMyObj(nullptr);
		m_ExpSE->Play(false, 1.0f);
		m_PropellerSE->SetDestroy();
		SetDestroy();
		player->SetHP(player->GetHP() - 1);
		scene->AddGameObject<Effect>(OBJ3D)->SetEffectAll(m_Position, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_Scale * 10, EFFECT_EXPLOSION);
	}
}

void Enemy::UpdateHeight()
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

void Enemy::UpdateSound()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	//プロペラ音調整
	if (MeasureDist(player->GetPos(), m_Position) > MAX_SE_DIST)
	{
		m_PropellerSE->SetVolume(0.0f);
	}

	else
	{
		m_PropellerSE->SetVolume((MAX_SE_DIST - (MeasureDist(player->GetPos(), m_Position))) / 10);
	}
}

void Enemy::UpdateTitleMove()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	m_TargetRotY = (-atan2f((player->GetPos().z - m_Position.z), (player->GetPos().x - m_Position.x)) + (D3DX_PI / 2));

	AdjustmentRadian(m_TargetRotY);

	m_Rotation.y = (float)LerpRadian(m_Rotation.y, m_TargetRotY, 0.5f);

	for (int i = 0; i < 4; i++)
	{
		propeller[i].Rot.y += 2.5f;
		AdjustmentRadian(propeller[i].Rot.y);
	}
}