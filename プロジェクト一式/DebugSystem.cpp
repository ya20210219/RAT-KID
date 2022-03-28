#include "DebugSystem.h"

void DebugSystem::Init()
{
	m_DebugUse = false;
}

void DebugSystem::Update()
{
	if (Keyboard_IsTrigger(DIK_F5))
	{
		if (m_DebugUse == false)
		{
			SetDebug(true);
			ShowCursor(true);
		}
	}
	if (Keyboard_IsTrigger(DIK_F6))
	{
		if (m_DebugUse == true)
		{
			SetDebug(false);
			ShowCursor(false);
		}
	}
}

void DebugSystem::UnInit()
{
}

void DebugSystem::Draw()
{
}