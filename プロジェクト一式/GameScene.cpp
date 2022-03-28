#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "GameScene.h"
#include "Camera.h"
#include "Field.h"
#include "MeshField.h"
#include "model.h"
#include "Player.h"
#include "polygon2D.h"
#include "audio.h"
#include "Radar.h"
#include "DebugSystem.h"
#include "OBB.h"
#include "Fade.h"
#include "Building.h"
#include "WireTarget.h"
#include "Wire.h"
#include "Coin.h"
#include "Score.h"
#include "Result.h"
#include "DebugForwardRay.h"
#include "WireUI.h"
#include "Sky.h"
#include "Enemy.h"
#include "HP.h"
#include "Residue.h"
#include "TextUI.h"
#include "Helicopter.h"
#include "ClearTex.h"

void GameScene::Init()
{
	OBB::Load();
	Building::Load();
	Coin::Load();
	Enemy::Load();
	Enemy::Propeller::Load();
	Helicopter::Load();
	Helicopter::Propeller::Load();

	Scene* scene = Manager::GetScene();

	AddGameObject<Camera>(SYSTEM);
	AddGameObject<DebugSystem>(SYSTEM);
	Audio* bgm = AddGameObject<Audio>(SYSTEM);
	bgm->Load("asset\\audio\\battle2.wav");
	bgm->Play(true, 0.25f);
	AddGameObject<MeshField>(OBJ3D);
	AddGameObject<Sky>(OBJ3D);

	int size;
	FILE* fp;
	fp = fopen("asset/ObjectData.txt", "r");            //ファイル読み込み
	fscanf(fp, "%d", &size);
	for (int i = 0; i < size; i++)
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int t;
		fscanf(fp, "%f,%f,%f", &pos.x, &pos.y, &pos.z);
		fscanf(fp, "%f,%f,%f", &rot.x, &rot.y, &rot.z);
		fscanf(fp, "%d", &t);
		AddGameObject<Building>(OBJ3D)->CreateBldg(pos, rot, static_cast<BldgType>(t));
	}

	fscanf(fp, "%d", &size);
	for (int i = 0; i < size; i++)
	{
		D3DXVECTOR3 pos;
		fscanf(fp, "%f,%f,%f", &pos.x, &pos.y, &pos.z);
		AddGameObject<Coin>(OBJ3D)->SetPos(pos.x, pos.y, pos.z);
	}

	fclose(fp);

	AddGameObject<MeshField>(OBJ3D);
	AddGameObject<Player>(OBJ3D);
	AddGameObject<Enemy>(OBJ3D)->SetPos(-60.0f, 1.0f, -15.0f);
	AddGameObject<Enemy>(OBJ3D)->SetPos(25.0f, 1.0f, -25.0f);
	AddGameObject<Enemy>(OBJ3D)->SetPos(-35.0f, 1.0f, 0.0f);
	AddGameObject<WireTarget>(OBJ3D);
	AddGameObject<DebugForwardRay>(OBJ3D);
	AddGameObject<Wire>(OBJ3D);
	AddGameObject<Radar>(OBJ2D);
	AddGameObject<Score>(OBJ2D);
	AddGameObject<Residue>(OBJ2D);
	AddGameObject<TextUI>(OBJ2D);
	AddGameObject<WireUI>(OBJ2D);
	AddGameObject<HP>(OBJ2D);
	m_Fade = scene->AddGameObject<Fade>(OBJ2D);

	m_Fade->SetFade(true);
	m_Fade->SetFadeIn(true, 1.0f);
	m_isNextScene = false;
	m_isClear = false;
}

void GameScene::UnInit()
{
	Scene::UnInit();//継承元
	Helicopter::Propeller::UnLoad();
	Helicopter::UnLoad();
	Enemy::Propeller::UnLoad();
	Enemy::UnLoad();
	OBB::UnLoad();
	Building::UnLoad();
	Coin::UnLoad();
}

void GameScene::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>(OBJ3D);
	Player* player = scene->GetGameObject<Player>(OBJ3D);
	Helicopter* heli = scene->GetGameObject<Helicopter>(OBJ3D);
	ClearTex* ct = scene->GetGameObject<ClearTex>(OBJ2D);
	std::vector<Coin*> coinList = scene->GetGameObjects<Coin>(OBJ3D);

	int count = coinList.size();

	if (count == 0 && heli == nullptr && ct == nullptr)
	{
		AddGameObject<Helicopter>(OBJ3D)->SetPos(250.0f, 250.0f, 250.0f);
		AddGameObject<ClearTex>(OBJ2D);
	}

	//デバッグ用
	if (Keyboard_IsTrigger(DIK_K))
	{
		AddGameObject<Helicopter>(OBJ3D)->SetPos(250.0f, 250.0f, 250.0f);
		AddGameObject<ClearTex>(OBJ2D);
	}

	if (heli != nullptr)
	{
		if (heli->GetOBB()->ColOBBs(*heli->GetOBB(), *player->GetOBB()) == true)
		{
			m_isClear = true;
		}
	}

	if (m_isClear == true && m_Fade->GetFadeUse() == false && m_Fade->GetFadeEnd() == true)
	{
		if (m_Fade->GetFadeStart() == false)
		{
			m_Fade->SetFade(true);
		}
		m_Fade->SetFadeOut(true, 0.0f);
	}

	else if (player->GetHP() <= 0 && m_Fade->GetFadeUse() == false && m_Fade->GetFadeEnd() == true)
	{
		if (m_Fade->GetFadeStart() == false)
		{
			m_Fade->SetFade(true);
		}
		m_Fade->SetFadeOut(true, 0.0f);
	}

	if (m_Fade->GetFadeOut() == true && m_Fade->GetFadeUse() == false)
	{
		Manager::SetScene<Result>();
	}
}

