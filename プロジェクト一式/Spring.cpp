#include "main.h"
#include "manager.h"
#include "Spring.h"
#include "Player.h"

void Spring::Init()
{
	Scene* scene = Manager::GetScene();
	m_Position = D3DXVECTOR3(-3.0f, DEFAULT_POSITION, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}
void Spring::UnInit()
{

}

void Spring::Update()
{
	D3DXVECTOR3 f;
	float dt = 0.05f;

	f.x = 0.0f;
	f.y = 0.0f;
	// í£óÕ					 
	f.x += (DEFAULT_POSITION - m_Position.x) * 1.0f;
	f.y += (DEFAULT_POSITION - m_Position.y) * 1.0f;
	f.z += (DEFAULT_POSITION - m_Position.z) * 1.0f;

	// èdóÕ
	f.y += (GRAVITY * 30) * PLAYER_MASS;

	// íÔçR
	f.x -= m_Velocity.x * 0.1f;
	f.y -= m_Velocity.y * 0.1f;
	f.z -= m_Velocity.z * 0.1f;

	// í£óÕÇ…ÇÊÇÈâ¡ë¨
	m_Velocity.x += (f.x / PLAYER_MASS) * dt;
	m_Velocity.y += (f.y / PLAYER_MASS) * dt;
	m_Velocity.z += (f.z / PLAYER_MASS) * dt;

	// à⁄ìÆ
	m_Position.x += m_Velocity.x * dt;
	m_Position.y += m_Velocity.y * dt;
	m_Position.z += m_Velocity.z * dt;
}

void Spring::Draw()
{

}