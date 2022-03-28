#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

#include <tchar.h>
#include "DebugSystem.h"
#include "Scene.h"
#include "player.h"
#include "Building.h"
#include "Coin.h"
#include "Radar.h"
#include "Camera.h"
#include "OBB.h"
#include "imguiManager.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


ID3D11Device* imguiManager::m_pd3dDevice = NULL;
ID3D11DeviceContext* imguiManager::m_pd3dDeviceContext = NULL;
IDXGISwapChain* imguiManager::m_pSwapChain = NULL;
ID3D11RenderTargetView* imguiManager::m_mainRenderTargetView = NULL;

void imguiManager::Init()
{
	m_pd3dDevice = Renderer::GetDevice();
	m_pd3dDeviceContext = Renderer::GetDeviceContext();
	m_mainRenderTargetView = Renderer::GetRenderTargetView();
	m_pSwapChain = Renderer::GetSwapChain();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	HWND window = GetWindow();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext);
}

void imguiManager::Update()
{
}

void imguiManager::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void imguiManager::Draw()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

	Scene* scene = Manager::GetScene();
	DebugSystem* d = scene->GetGameObject<DebugSystem>(SYSTEM);
	bool show;
	if (d != nullptr)
	{
		show = d->GetDebug();
	}
	else
	{
		show = false;
	}

	// ‚±‚±‚©‚ç
	if (show == true)
	{
		if (ImGui::Begin("debug", &show))
		{
			ImGui::SetNextWindowSize(ImVec2(300, 50), ImGuiCond_Once);
			ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
			Scene* scene = Manager::GetScene();
			SCENE_NAME name = scene->GetSceneName();

			ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			ImGui::Text("SCENENAME = %d", name);
			if (ImGui::Button("Quit"))
			{
				d->SetDebug(false);
				return;
			}
			ImGui::End();
		}


		if (ImGui::Begin("PlayerStatus", &show))
		{
			ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
			ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
			Scene* scene = Manager::GetScene();
			Player* player = scene->GetGameObject<Player>(OBJ3D);

			ImGui::Text("Player");
			ImGui::Text("X = %f\nY = %f\nZ = %f\n", player->GetPos().x, player->GetPos().y, player->GetPos().z);
			ImGui::Text("m_isFly = %s\nm_isCol = %s\nm_isGravity = %s\nm_isWire = %s", (player->GetisFly() ? "true" : "false"),
				(player->GetisCol() ? "true" : "false"),
				(player->GetisGravity() ? "true" : "false"),
				(player->GetisWire() ? "true" : "false"));
			ImGui::Text("AniTypeNum = %d", player->GetAniType());
			ImGui::Text("X = %f\nY = %f\nZ = %f\n", player->GetTargetPos().x, player->GetTargetPos().y, player->GetTargetPos().z);
			ImGui::End();
		}

		if (ImGui::Begin("CameraStatus", &show))
		{
			ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
			ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
			Scene* scene = Manager::GetScene();
			Camera* cam = scene->GetGameObject<Camera>(SYSTEM);

			ImGui::Text("CameraRot");
			ImGui::Text("X = %f\nY = %f\nZ = %f\n", cam->GetRot().x, cam->GetRot().y, cam->GetRot().z);
			ImGui::End();
		}

		if (ImGui::Begin("BldgStatus", &show))
		{
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(200, 200), ImGuiWindowFlags_NoTitleBar);
			std::vector<Building*> BuildingList = scene->GetGameObjects<Building>(OBJ3D);

			for (Building* bldg : BuildingList)
			{
				ImGui::Text("X = %f\nY = %f\nZ = %f\n", bldg->GetPos().x, bldg->GetPos().y, bldg->GetPos().z);
				ImGui::Text("RoofTop = %f", bldg->GetPos().y + bldg->GetOBB()->GetLen_W(1));
			}
			ImGui::EndChild();

			ImGui::End();
		}

		if (ImGui::Begin("CoinStatus", &show))
		{
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 200), ImGuiWindowFlags_NoTitleBar);
			std::vector<Coin*> CoinList = scene->GetGameObjects<Coin>(OBJ3D);

			for (Coin* coin : CoinList)
			{
				ImGui::Text("X = %f\nY = %f\nZ = %f\n", coin->GetPos().x, coin->GetPos().y, coin->GetPos().z);
			}
			ImGui::EndChild();

			ImGui::End();
		}

		if (ImGui::Begin("RadarDotStatus", &show))
		{
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 200), ImGuiWindowFlags_NoTitleBar);
			std::vector<RadarDot*> RadarDotList = scene->GetGameObjects<RadarDot>(OBJ2D);

			for (RadarDot* dot : RadarDotList)
			{
				ImGui::Text("X = %f\nY = %f\nZ = %f\n", dot->GetPos().x, dot->GetPos().y, dot->GetPos().z);
			}
			ImGui::EndChild();

			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
