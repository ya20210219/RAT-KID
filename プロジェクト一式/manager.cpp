#include "main.h"
#include "GameObject.h"
#include "manager.h"
#include "Input.h"
#include "renderer.h"
#include "Scene.h"
#include "GameScene.h"
#include "Title.h"
#include "audio.h"

#include "imguiManager.h"

Scene* Manager::m_Scene = nullptr;

void Manager::Init()
{
	Renderer::Init();
	Audio::InitMaster();

	imguiManager::Init();

	SetScene<Title>();
}


void Manager::Uninit()
{
	m_Scene->UnInit();
	delete m_Scene;

	Audio::UninitMaster();

	UninitInput();
	Renderer::Uninit();

	imguiManager::Uninit();
}

void Manager::Update()
{
	UpdateInput();
	imguiManager::Update();

	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	imguiManager::Draw();

	Renderer::End();
}

