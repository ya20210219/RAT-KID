#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "Camera.h"
#include "GameObject.h"
#include "model.h"
#include "Player.h"
#include "Input.h"
#include "Calculation.h"
#include "OBB.h"
#include "Scene.h"
#include "Manager.h"
#include "Building.h"

void Camera::Init()
{
	Scene* scene = Manager::GetScene();
	m_Position = D3DXVECTOR3(0.0f, 2.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());			//x軸の向き
	m_obb->SetDirect(1, GetUp());				//y軸の向き
	m_obb->SetDirect(2, GetForward()); //z軸の向き
	m_obb->SetLen_W(0, 0.1f);	//x軸での厚み
	m_obb->SetLen_W(1, 0.1f);	//y軸での厚み
	m_obb->SetLen_W(2, 0.1f);	//z軸での厚み

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);
}

void Camera::UnInit()
{
}

void Camera::Update()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	SCENE_NAME name = scene->GetSceneName();

	if (name == SCENE_TITLE)
	{
		m_Rotation.y = AdjustmentRadian(player->GetRot().y + D3DX_PI);
	}

	if (name == SCENE_GAMESCENE || name == SCENE_TUTORIAL)
	{
		m_CameraMove.x = static_cast<float>(Mouse_GetMoveX());
		m_Rotation.y += (m_CameraMove.x / 1000);
		m_Rotation.y = AdjustmentRadian(m_Rotation.y);

		m_CameraMove.y = static_cast<float>(Mouse_GetMoveY());
		m_Rotation.x += (m_CameraMove.y / 1000);
		m_Rotation.x = AdjustmentRadian(m_Rotation.x);

	}

	if (m_Rotation.x < (-D3DX_PI / 2))
	{
		m_Rotation.x = (-D3DX_PI / 2);
	}

	if (m_Rotation.x > (D3DX_PI / 2))
	{
		m_Rotation.x = (D3DX_PI / 2);
	}

	//徐々にプレイヤー正面を向く
	if (Keyboard_IsPress(DIK_R))
	{
		m_Rotation.y = (float)LerpRadian(m_Rotation.y, AdjustmentRadian(player->GetRot().y + D3DX_PI), 0.5f);
	}

	m_Direction = GetForward();

	UpdateOBB();
}

void Camera::Draw()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	D3DXMATRIX mat_move;
	D3DXMatrixTranslation(&mat_move, m_Position.x, m_Position.y, m_Position.z);

	D3DXMATRIX mat_rot;
	D3DXMatrixRotationYawPitchRoll(&mat_rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

	D3DXMATRIX mat_rotmove;
	mat_rotmove = mat_rot * mat_move;

	D3DXMatrixInverse(&m_ViewMatrix, NULL, &mat_rotmove);//カメラの逆行列を求める

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);

	Renderer::SetCameraPosition(m_Position);
}

void Camera::UpdateOBB()
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

	Scene* scene = Manager::GetScene();
	std::vector<Building*> BuildingList = scene->GetGameObjects<Building>(OBJ3D);
	Player* player = scene->GetGameObject<Player>(OBJ3D);

	m_Target = D3DXVECTOR3(player->GetPos().x, player->GetPos().y + 2.0f, player->GetPos().z);
	m_Position = m_Target + GetForward() * CAMERA_DIST;

	for (Building* bldg : BuildingList)
	{
		if (m_obb->ColOBBs(*m_obb, *bldg->GetOBB()))
		{
			m_Position = bldg->CheckMeshToCam();
			m_Target = m_Position + -GetForward() * MeasureDist(m_Position, player->GetPos());
			break;
		}
	}

	m_Direction = m_Target - m_Position;
	D3DXVec3Normalize(&m_Direction, &m_Direction);
}

//視錐台カリング
bool Camera::CheckView(D3DXVECTOR3 pos, OBB* obb)
{
	D3DXMATRIX vp, invvp;

	D3DXVECTOR3 targetPos = pos;

	vp = m_ViewMatrix * m_ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, -1.0f);

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;

	v = pos - m_Position;

	//左面=========================================
	v1 = wpos[0] - m_Position;
	v2 = wpos[2] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	if (D3DXVec3Dot(&n, &v) < -obb->GetLen_W(0))
	{
		return false;
	}
	//右面=========================================
	v1 = wpos[1] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	if (D3DXVec3Dot(&n, &v) > obb->GetLen_W(0))
	{
		return false;
	}
	//上面=========================================
	v1 = wpos[0] - m_Position;
	v2 = wpos[1] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	if (D3DXVec3Dot(&n, &v) > obb->GetLen_W(1))
	{
		return false;
	}
	//下面=========================================
	v1 = wpos[2] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	if (D3DXVec3Dot(&n, &v) < -obb->GetLen_W(1))
	{
		return false;
	}

	return true;
}
