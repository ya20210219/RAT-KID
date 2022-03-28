#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Tutorial.h"
#include "audio.h"
#include "Camera.h"
#include "Field.h"
#include "MeshField.h"
#include "model.h"
#include "Player.h"
#include "polygon2D.h"
#include "Radar.h"
#include "DebugSystem.h"
#include "OBB.h"
#include "Fade.h"
#include "Building.h"
#include "WireTarget.h"
#include "Wire.h"
#include "WireUI.h"
#include "Coin.h"
#include "Score.h"
#include "GameScene.h"
#include "TutorialLogo.h"
#include "Sky.h"
#include "WASD.h"
#include "MouseTex.h"
#include "Helicopter.h"
#include "DebugForwardRay.h"

void Tutorial::Init()
{
	OBB::Load();

	Building::Load();

	Coin::Load();

	Scene* scene = Manager::GetScene();

	AddGameObject<Camera>(SYSTEM);
	AddGameObject<DebugSystem>(SYSTEM);

	AddGameObject<MeshField>(OBJ3D);
	AddGameObject<Sky>(OBJ3D);

	AddGameObject<Building>(OBJ3D)->CreateBldg(D3DXVECTOR3(-3.0f, 0.0f, 6.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BLDG_TYPE_NORMAL);
	AddGameObject<Coin>(OBJ3D)->SetPos(-3.0f, 10.0f, 6.0f);

	AddGameObject<MeshField>(OBJ3D);

	AddGameObject<Player>(OBJ3D);

	AddGameObject<WireTarget>(OBJ3D);

	AddGameObject<DebugForwardRay>(OBJ3D);

	AddGameObject<Wire>(OBJ3D);

	AddGameObject<Radar>(OBJ2D);

	AddGameObject<WireUI>(OBJ2D);
	AddGameObject<TutorialLogo>(OBJ2D);
	AddGameObject<WASD>(OBJ2D);
	AddGameObject<MouseTex>(OBJ2D);

	//Audio* bgm = AddGameObject<Audio>(SYSTEM);
	//bgm->Load("asset\\audio\\Select.wav");
	//bgm->Play(true);

	m_Fade = scene->AddGameObject<Fade>(OBJ2D);

	m_Fade->SetFade(true);
	m_Fade->SetFadeIn(true, 1.0f);
}

void Tutorial::UnInit()
{
	Scene::UnInit();//Œp³Œ³

	OBB::UnLoad();

	Building::UnLoad();

	Coin::UnLoad();
}

void Tutorial::Update()
{
	Scene::Update();

	if (Keyboard_IsPress(DIK_RETURN) && m_Fade->GetFadeUse() == false && m_Fade->GetFadeEnd() == true)
	{
		m_Fade->SetFadeEnd(false);
		m_Fade->SetFade(true);
		m_Fade->SetFadeOut(true, 0.0f);
	}
	if (m_Fade->GetFadeStart() == true)
	{
		if (m_Fade->GetFadeEnd() == true)
		{
			m_Fade->SetFadeEnd(false);
			Manager::SetScene<GameScene>();
		}
	}
}

