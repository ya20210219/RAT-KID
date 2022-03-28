#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "GameObject.h"
#include "Building.h"
#include "Calculation.h"
#include "MeshField.h"
#include "OBB.h"
#include "Player.h"
#include "Camera.h"
#include "WireTarget.h"
#include "Input.h"

Model* Building::m_Model[3];

void Building::Init()
{
	Scene* scene = Manager::GetScene();
	MeshField* meshField = scene->GetGameObject<MeshField>(OBJ3D);

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "pixelLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "pixelLightingPS.cso");

	m_Frame = 0;
	m_BlendRate = 0;
}

void Building::UnInit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_obb->SetDestroy();
}

void Building::Update()
{
	//高さ更新
	UpdateHeight();
	UpdateOBB();

	//旧座標更新
	m_OldPosition = m_Position;

	m_Frame++;
}

void Building::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* cam = scene->GetGameObject<Camera>(0);

	//if (!cam->CheckView(m_Position, m_obb))
	//{
	//	return;
	//}

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
	m_EnemyMatrix = scale * rot * trans;

	Renderer::SetWorldMatrix(&m_EnemyMatrix);

	m_Model[static_cast<int>(m_Type)]->Draw();
}

void Building::UpdateHeight()
{
	if (m_isCorrection == false)
	{
		Scene* scene = Manager::GetScene();
		MeshField* meshField = scene->GetGameObject<MeshField>(OBJ3D);

		//地面の高さ取得
		float y = meshField->GetHeight(m_Position);
		m_Position.y = y + m_obb->GetLen_W(1);
		//obbサイズを微調整しているため、小数点以下を切り捨てる
		m_Position.y = std::floor(m_Position.y);

		m_isCorrection = true;
	}
}

void Building::UpdateOBB()
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

//ワイヤー用。レイとメッシュの交差判定
void Building::CheckMeshToRay()
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
	maxnum = m_Model[static_cast<int>(m_Type)]->GetIndexNum();

	for (unsigned int i = 0; i < maxnum; i += 3)
	{
		VERTEX_3D *vertex = m_Model[static_cast<int>(m_Type)]->GetVertexArray();
		unsigned int *nownum = m_Model[static_cast<int>(m_Type)]->GetIndexArray();

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

D3DXVECTOR3 Building::CheckMeshToCam()
{
	float RayDistance;
	float RecentRayDistance = 1000.0f;
	bool bRayHit;
	D3DXVECTOR3 wPos[3];
	D3DXVECTOR3 wDir;

	Scene* scene = Manager::GetScene();
	Camera* cam = scene->GetGameObject<Camera>(SYSTEM);

	unsigned int maxnum;
	maxnum = m_Model[static_cast<int>(m_Type)]->GetIndexNum();

	for (unsigned int i = 0; i < maxnum; i += 3)
	{
		VERTEX_3D *vertex = m_Model[static_cast<int>(m_Type)]->GetVertexArray();
		unsigned int *nownum = m_Model[static_cast<int>(m_Type)]->GetIndexArray();

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
		DirectX::XMVECTOR dir = DirectX::XMVectorSet(cam->GetForward().x, cam->GetForward().y, cam->GetForward().z, 0.0f);

		DirectX::XMVECTOR v0 = DirectX::XMVectorSet(pos0.x, pos0.y, pos0.z, 0.0f);
		DirectX::XMVECTOR v1 = DirectX::XMVectorSet(pos1.x, pos1.y, pos1.z, 0.0f);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSet(pos2.x, pos2.y, pos2.z, 0.0f);

		bRayHit = DirectX::TriangleTests::Intersects(pos, DirectX::XMVector3Normalize(dir), v0, v1, v2, RayDistance);
		if (bRayHit)
		{
			//すべての三角形を調べて、一番近い所を採用
			//当たった点との距離で一番短い物を衝突点との距離として採用する			
			if (RecentRayDistance > RayDistance)
			{
				RecentRayDistance = RayDistance;
				D3DXVECTOR3 pos = cam->GetPos() + cam->GetForward() * (RecentRayDistance + 0.05f);

				return pos;
			}
		}
	}

	return  cam->GetPos();
}


//足元
bool Building::CheckFootMeshToRay(D3DXVECTOR3 dir)
{
	float RayDistance = 1.0f;
	bool bRayHit;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	Camera* cam = scene->GetGameObject<Camera>(SYSTEM);
	WireTarget* wt = scene->GetGameObject<WireTarget>(OBJ3D);

	unsigned int maxnum;
	maxnum = m_Model[static_cast<int>(m_Type)]->GetIndexNum();

	for (unsigned int i = 0; i < maxnum; i += 3)
	{
		VERTEX_3D *vertex = m_Model[static_cast<int>(m_Type)]->GetVertexArray();
		unsigned int *nownum = m_Model[static_cast<int>(m_Type)]->GetIndexArray();

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
		DirectX::XMVECTOR pos = DirectX::XMVectorSet(player->GetPos().x, player->GetPos().y, player->GetPos().z, 0.0f);

		//レイを飛ばす方向
		DirectX::XMVECTOR pDir = DirectX::XMVectorSet(dir.x, dir.y, dir.z, 0.0f);

		DirectX::XMVECTOR v0 = DirectX::XMVectorSet(pos0.x, pos0.y, pos0.z, 0.0f);
		DirectX::XMVECTOR v1 = DirectX::XMVectorSet(pos1.x, pos1.y, pos1.z, 0.0f);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSet(pos2.x, pos2.y, pos2.z, 0.0f);

		bRayHit = DirectX::TriangleTests::Intersects(pos, DirectX::XMVector3Normalize(pDir), v0, v1, v2, RayDistance);
		if (bRayHit == true && RayDistance < HUGE_EDGE)
		{
			return bRayHit;
		}
	}

	return false;
}

//壁との当たり判定用
D3DXVECTOR3 Building::CheckWallMeshToRay(GameObject* obj)
{
	float RayDistance;
	float RecentRayDistance = 1000.0f;
	bool bRayHit;
	D3DXVECTOR3 wPos[3];
	D3DXVECTOR3 wDir;

	float Add;

	unsigned int maxnum;
	maxnum = m_Model[static_cast<int>(m_Type)]->GetIndexNum();

	for (int n = 0; n < 2; n++)
	{
		for (unsigned int i = 0; i < maxnum; i += 3)
		{
			VERTEX_3D *vertex = m_Model[static_cast<int>(m_Type)]->GetVertexArray();
			unsigned int *nownum = m_Model[static_cast<int>(m_Type)]->GetIndexArray();
		
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
			DirectX::XMVECTOR pos = DirectX::XMVectorSet(obj->GetPos().x, obj->GetPos().y + obj->GetOBB()->GetLen_W(1), obj->GetPos().z, 0.0f);

			//レイを飛ばす方向
			DirectX::XMVECTOR Raydir;
			if (n == 0)
			{
				Raydir = DirectX::XMVectorSet(-obj->GetDirection().x, -obj->GetDirection().y, -obj->GetDirection().z, 0.0f);
			}
			if (n == 1)
			{
				Raydir = DirectX::XMVectorSet(obj->GetDirection().x, obj->GetDirection().y, obj->GetDirection().z, 0.0f);
			}

			DirectX::XMVECTOR v0 = DirectX::XMVectorSet(pos0.x, pos0.y, pos0.z, 0.0f);
			DirectX::XMVECTOR v1 = DirectX::XMVectorSet(pos1.x, pos1.y, pos1.z, 0.0f);
			DirectX::XMVECTOR v2 = DirectX::XMVectorSet(pos2.x, pos2.y, pos2.z, 0.0f);

			bRayHit = DirectX::TriangleTests::Intersects(pos, DirectX::XMVector3Normalize(Raydir), v0, v1, v2, RayDistance);
			if (bRayHit)
			{
				//すべてのポリゴンを調べて、一番近い所を採用
				//当たった点との距離で一番短い物を衝突点との距離として採用する
				if (RecentRayDistance > RayDistance)
				{
					RecentRayDistance = RayDistance;

					if (RecentRayDistance < 1.0f)
					{
						wPos[0] = D3DXVECTOR3(pos0.x, pos0.y, pos0.z);
						wPos[1] = D3DXVECTOR3(pos1.x, pos1.y, pos1.z);
						wPos[2] = D3DXVECTOR3(pos2.x, pos2.y, pos2.z);

						D3DXVECTOR3 v10 = wPos[1] - wPos[0];
						D3DXVECTOR3 v12 = wPos[2] - wPos[1];

						wDir = *D3DXVec3Cross(&wDir, &v10, &v12);
						D3DXVec3Normalize(&wDir, &wDir);

						float r = 0.0f;
						for (i = 0; i < 3; i++)
						{
							D3DXVECTOR3 Direct = obj->GetOBB()->GetDirect(i); // OBBの1つの軸ベクトル
							r += fabs(D3DXVec3Dot(&(Direct * obj->GetOBB()->GetLen_W(i)), &wDir));
						}

						D3DXVECTOR3 ObbPos = obj->GetOBB()->GetPos_W();
						D3DXVECTOR3 PlanePos = (wPos[0] + wPos[1] + wPos[2]) / 3;
						float s = D3DXVec3Dot(&(ObbPos - PlanePos), &wDir);

						if (s > 0)
						{
							Add = r - fabs(s);
						}
						else
						{
							Add = r + fabs(s);
						}

						//オブジェクトを当たっている平面の方向へ補正値分ずらす
						D3DXVECTOR3 p = obj->GetPos() + wDir * (Add + 0.1f);
						p = D3DXVECTOR3(p.x, obj->GetPos().y, p.z);

						return p;
					}
				}
			}
		}
	}

	return obj->GetPos();
}



void Building::CreateBldg(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BldgType t)
{
	Scene* scene = Manager::GetScene();
	m_Position = pos;
	m_Rotation = rot;
	m_Type = t;

	m_obb = scene->AddGameObject<OBB>(OBJ3D);
	m_obb->SetOBBPos(D3DXVECTOR3(m_Position.x, m_Position.y, m_Position.z));
	m_obb->SetDirect(0, GetRight());
	m_obb->SetDirect(1, GetUp());
	m_obb->SetDirect(2, GetForward());

	switch (m_Type)
	{
	case BLDG_TYPE_NORMAL:
		m_obb->SetLen_W(0, 5.1f);
		m_obb->SetLen_W(1, 10.1f);
		m_obb->SetLen_W(2, 5.1f);
		m_AdjustmentEdgeHeight = NOMAL_EDGE;
		break;
	case BLDG_TYPE_LARGE:
		m_obb->SetLen_W(0, 5.1f);
		m_obb->SetLen_W(1, 20.1f);
		m_obb->SetLen_W(2, 5.1f);
		m_AdjustmentEdgeHeight = LARGE_EDGE;
		break;
	case BLDG_TYPE_HUGE:
		m_obb->SetLen_W(0, 20.5f);
		m_obb->SetLen_W(1, 40.5f);
		m_obb->SetLen_W(2, 20.5f);
		m_AdjustmentEdgeHeight = HUGE_EDGE;
		break;
	}

	m_obb->SetAll(D3DXVECTOR3(m_obb->GetPos_W().x, m_obb->GetPos_W().y, m_obb->GetPos_W().z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(m_obb->GetLen_W(0), m_obb->GetLen_W(1), m_obb->GetLen_W(2)),
		0.0f,
		0.0f);

	m_isCorrection = false;
}