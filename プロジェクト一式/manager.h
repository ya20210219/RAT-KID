#pragma once

#include "Scene.h"
#include "Title.h"
#include "Tutorial.h"
#include "GameScene.h"
#include "Result.h"
#include <string>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


class Manager
{
private:
	static class Scene* m_Scene;	//頭にclassとつけると読み込める。前方宣言という
	static float m_Timer;
	static float m_isTimer;

public:	//メソッド、メンバ関数

	static void Init();		//初期化処理
	static void Uninit();	//終了処理
	static void Update();	//更新処理
	static void Draw();		//描画処理

	static Scene* GetScene() { return m_Scene; }

	template<typename T>
	static void SetScene()
	{
		int score;
		if (m_Scene)
		{
			score = m_Scene->GetScore();
			m_Scene->UnInit();
			delete m_Scene;
		}

		T* scene = new T();
		m_Scene = scene;

		const type_info& id = typeid(*scene);

		if (typeid(Title) == id)
		{
			m_Scene->SetSceneName(SCENE_TITLE);
		}
		if (typeid(Tutorial) == id)
		{
			m_Scene->SetSceneName(SCENE_TUTORIAL);
		}
		if (typeid(GameScene) == id)
		{
			m_Scene->SetSceneName(SCENE_GAMESCENE);
		}
		if (typeid(Result) == id)
		{
			m_Scene->SetSceneName(SCENE_RESULT);
			m_Scene->SetScore(score);
		}
		scene->Init();
	}

	void StartTimer(bool enable)
	{
		m_isTimer = enable;
	}
};