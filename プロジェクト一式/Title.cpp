#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Title.h"
#include "TitleLogo.h"
#include "Fade.h"
#include "Input.h"
#include "Tutorial.h"
#include "GameScene.h"
#include "Sky.h"
#include "Building.h"
#include "Camera.h"
#include "MeshField.h"
#include "Player.h"
#include "Wire.h"
#include "WireTarget.h"
#include "Spring.h"
#include "DebugSystem.h"
#include "Enemy.h"
#include "Particle.h"
#include "PressEnter.h"
#include "OBB.h"

void Title::Init()
{
	RECT rc;
	GetWindowRect(GetWindow(), &rc);
	SetCursorPos(rc.left, rc.top);

	//カーソル範囲指定
	POINT p;
	GetCursorPos(&p);

	rc.left = p.x + 10;
	rc.top = p.y + 35;						// 左上隅のY座標
	rc.right = p.x + SCREEN_WIDTH - 5;		// 右下隅のX座標
	rc.bottom = p.y + SCREEN_HEIGHT + 15;      // 右下隅のY座標
	ClipCursor(&rc);

	Scene* scene = Manager::GetScene();

	OBB::Load();
	Building::Load();
	Enemy::Load();
	Enemy::Propeller::Load();

	AddGameObject<Sky>(OBJ3D);

	AddGameObject<Camera>(SYSTEM);
	AddGameObject<DebugSystem>(SYSTEM);
	AddGameObject<MeshField>(OBJ3D);

	//AddGameObject<Building>(OBJ3D)->SetPos(-3.0f, 0.0f, 5.0f);
	AddGameObject<Building>(OBJ3D)->CreateBldg(D3DXVECTOR3(-3.0f, 0.0f, 5.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLDG_TYPE_NORMAL);
	AddGameObject<Player>(OBJ3D);

	AddGameObject<WireTarget>(OBJ3D);

	AddGameObject<Wire>(OBJ3D);
	AddGameObject<Spring>(OBJ3D);
	AddGameObject<Enemy>(OBJ3D)->SetPos(-15.0f, 5.0f, -10.0f);

	AddGameObject<Sky>(OBJ3D);
	AddGameObject<TitleLogo>(OBJ2D);

	for (int i = 0; i < 50; i++)
	{
		AddGameObject<Particle>(OBJ2D)->SetVel(-51 + rand() % 101 - 5, rand() % 11 - 5);
	}

	m_Fade = scene->AddGameObject<Fade>(OBJ2D);
	m_isNextScene = false;
}

void Title::UnInit()
{
	Scene::UnInit();//継承元

	Enemy::Propeller::UnLoad();
	Enemy::UnLoad();
	Building::UnLoad();
	OBB::UnLoad();
}

void Title::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();

	if (Keyboard_IsPress(DIK_RETURN) || Keyboard_IsPress(DIK_SPACE))
	{
		if (m_isNextScene == false)
		{
			m_isNextScene = true;
			m_Fade->SetFadeEnd(false);
			m_Fade->SetFade(true);
			m_Fade->SetFadeOut(true, 0.0f);
		}
	}	

	if (m_Fade->GetFadeStart() == true)
	{
		if (m_Fade->GetFadeEnd() == true)
		{
			m_Fade->SetFadeEnd(false);
			Manager::SetScene<Tutorial>();
		}
	}
}